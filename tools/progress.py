#!/usr/bin/env python3
"""Print decomp progress and write an objdiff-style report.

Stdout is the project JSON (version 1, code/data totals).
--report writes build/report.json in the objdiff report shape that decomp.dev
reads from a GitHub Actions artifact named ``<version>_report``.
Matched counts stay at 0 until a linker map records matched addresses.
"""

import argparse
import json
import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
FACTS = os.path.join(ROOT, "config", "sum-pc.json")


def load_facts():
    with open(FACTS, "r", encoding="utf-8") as handle:
        return json.load(handle)


def section(facts, name):
    for item in facts["sections"]:
        if item["name"] == name:
            return item
    raise KeyError(name)


def totals(facts):
    code = section(facts, ".text")["vsize"]
    data = (
        section(facts, ".rdata")["vsize"]
        + section(facts, ".data")["rawsize"]
        + section(facts, ".rsrc")["vsize"]
    )
    return code, data


def matched_from_map(map_path):
    """Return matched code and data bytes once a map marks them.

    A future matching build can append lines of the form
    ``MATCH code <bytes>`` and ``MATCH data <bytes>``. Until those exist the
    retail map, if any, contributes nothing.
    """
    code = 0
    data = 0
    if not os.path.isfile(map_path):
        return code, data
    with open(map_path, "r", encoding="utf-8", errors="replace") as handle:
        for line in handle:
            parts = line.split()
            if len(parts) == 3 and parts[0] == "MATCH" and parts[1] in ("code", "data"):
                if parts[1] == "code":
                    code = int(parts[2])
                else:
                    data = int(parts[2])
    return code, data


def metrics(code_total, data_total, code_matched, data_matched):
    def pct(matched, total):
        if total <= 0:
            return 0.0
        return round(100.0 * matched / total, 4)

    return {
        "version": 1,
        "metrics": {
            "code": {
                "total": code_total,
                "matched": code_matched,
                "percentage": pct(code_matched, code_total),
            },
            "data": {
                "total": data_total,
                "matched": data_matched,
                "percentage": pct(data_matched, data_total),
            },
        },
    }


def measures(code_total, data_total, code_matched, data_matched, units, complete_units):
    def pct(matched, total):
        if total <= 0:
            return 0.0
        return 100.0 * matched / total

    return {
        "fuzzy_match_percent": pct(code_matched + data_matched, code_total + data_total),
        "total_code": str(code_total),
        "matched_code": str(code_matched),
        "matched_code_percent": pct(code_matched, code_total),
        "total_data": str(data_total),
        "matched_data": str(data_matched),
        "matched_data_percent": pct(data_matched, data_total),
        "total_functions": 0,
        "matched_functions": 0,
        "matched_functions_percent": 0.0,
        "complete_code": str(code_matched),
        "complete_code_percent": pct(code_matched, code_total),
        "complete_data": str(data_matched),
        "complete_data_percent": pct(data_matched, data_total),
        "total_units": units,
        "complete_units": complete_units,
    }


def load_symbol_groups():
    """Return (title, byte_count) for each section comment in symbols.txt.

    A size that runs into the next symbol is clipped so overlapping entries
    are not counted twice. Bytes are identified, not matched.
    """
    path = os.path.join(ROOT, "config", "symbols.txt")
    title = "Recovered"
    pending = None
    groups = []
    spans = []

    def close_group():
        if not spans:
            return
        ordered = sorted(spans)
        total = 0
        for index, (start, size) in enumerate(ordered):
            end = start + size
            if index + 1 < len(ordered):
                end = min(end, ordered[index + 1][0])
            if end > start:
                total += end - start
        groups.append((title, total))

    with open(path, "r", encoding="utf-8") as handle:
        for raw in handle:
            line = raw.split("#", 1)[0].strip()
            comment = raw.split("#", 1)[1].strip() if "#" in raw else ""
            if not line:
                if comment:
                    if spans:
                        close_group()
                        spans = []
                    pending = comment.split(".", 1)[0].strip()
                continue
            parts = line.split()
            if len(parts) < 3:
                continue
            if pending:
                title = pending
                pending = None
            spans.append((int(parts[0], 16), int(parts[1], 16)))
    close_group()
    return groups


def load_modules():
    path = os.path.join(ROOT, "config", "modules.txt")
    modules = []
    with open(path, "r", encoding="utf-8") as handle:
        for line in handle:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            modules.append(line)
    return modules


def category_for(original_path):
    lowered = original_path.replace("\\", "/").lower()
    if "/vsdk/" in lowered:
        return "vsdk"
    if "/levelscripts/" in lowered:
        return "scripts"
    if "/summoner/" in lowered:
        return "game"
    return "engine"


def load_text_symbols():
    """Every .text symbol, named or not. Sizes already cover the section."""
    import re

    path = os.path.join(ROOT, "config", "symbols_all.txt")
    pattern = re.compile(
        r"(\S+) = \.text:0x([0-9A-Fa-f]+); // type:function size:0x([0-9A-Fa-f]+)"
    )
    rows = []
    if not os.path.isfile(path):
        return rows
    with open(path, "r", encoding="utf-8") as handle:
        for raw in handle:
            match = pattern.match(raw.strip())
            if match:
                name, _address, size = match.groups()
                rows.append((name, int(size, 16)))
    return rows


def report(facts, code_total, data_total, code_matched, data_matched):
    modules = load_modules()
    rdata = section(facts, ".rdata")["vsize"]
    data_init = section(facts, ".data")["rawsize"]
    rsrc = section(facts, ".rsrc")["vsize"]
    # One tile per function, including functions that only have an address.
    # Matched stays 0 until a rebuild compares equal.
    units = []
    for name, size in load_text_symbols():
        units.append(
            {
                "name": name,
                "measures": measures(size, 0, 0, 0, 1, 0),
                "sections": [],
                "functions": [],
                "metadata": {"complete": False, "progress_categories": ["engine"]},
            }
        )
    units.extend(
        [
        {
            "name": "sum.exe:.rdata",
            "measures": measures(0, rdata, 0, 0, 1, 0),
            "sections": [],
            "functions": [],
            "metadata": {"complete": False, "progress_categories": ["engine"]},
        },
        {
            "name": "sum.exe:.data",
            "measures": measures(0, data_init, 0, 0, 1, 0),
            "sections": [],
            "functions": [],
            "metadata": {"complete": False, "progress_categories": ["engine"]},
        },
        {
            "name": "sum.exe:.rsrc",
            "measures": measures(0, rsrc, 0, 0, 1, 0),
            "sections": [],
            "functions": [],
            "metadata": {"complete": False, "progress_categories": ["engine"]},
        },
        ]
    )
    for original in modules:
        rel = original.split("pccode\\", 1)[-1].replace("\\", "/")
        units.append(
            {
                "name": rel,
                "measures": measures(0, 0, 0, 0, 1, 0),
                "sections": [],
                "functions": [],
                "metadata": {
                    "complete": False,
                    "source_path": "src/" + rel,
                    "progress_categories": [category_for(original)],
                },
            }
        )
    overall = measures(code_total, data_total, code_matched, data_matched, len(units), 0)
    engine = measures(code_total, data_total, code_matched, data_matched, 4, 0)
    empty = measures(0, 0, 0, 0, 0, 0)
    return {
        "measures": overall,
        "units": units,
        "version": 1,
        "categories": [
            {"id": "engine", "name": "Engine", "measures": engine},
            {"id": "vsdk", "name": "Volition SDK", "measures": empty},
            {"id": "game", "name": "Game", "measures": empty},
            {"id": "scripts", "name": "Level scripts", "measures": empty},
        ],
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--report", help="Also write an objdiff report JSON to this path")
    parser.add_argument("--map", default=os.path.join("build", "Sum.map"))
    args = parser.parse_args()

    facts = load_facts()
    code_total, data_total = totals(facts)
    code_matched, data_matched = matched_from_map(os.path.join(ROOT, args.map) if not os.path.isabs(args.map) else args.map)
    payload = metrics(code_total, data_total, code_matched, data_matched)
    json.dump(payload, sys.stdout, indent=2)
    sys.stdout.write("\n")

    if args.report:
        destination = args.report if os.path.isabs(args.report) else os.path.join(ROOT, args.report)
        os.makedirs(os.path.dirname(destination), exist_ok=True)
        with open(destination, "w", encoding="utf-8") as handle:
            json.dump(report(facts, code_total, data_total, code_matched, data_matched), handle, indent=2)
            handle.write("\n")


if __name__ == "__main__":
    main()
