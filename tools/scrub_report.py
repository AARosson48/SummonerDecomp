#!/usr/bin/env python3
"""Correct an objdiff report before decomp.dev reads it.

objdiff sets matched_data_percent and complete_data_percent to 100 when a
unit's data total is 0. Those fields are removed. Function virtual addresses
come from config/symbols_all.txt, then from the address in the symbol name.
This script does not assign a match score.
"""

import json
import os
import re
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
DATA_KEYS = (
    "matched_data",
    "matched_data_percent",
    "complete_data",
    "complete_data_percent",
)
CODE_KEYS = (
    "fuzzy_match_percent",
    "matched_code",
    "matched_code_percent",
    "complete_code",
    "complete_code_percent",
    "matched_functions",
    "matched_functions_percent",
)
SYMBOL = re.compile(
    r"(\S+) = \.\w+:0x([0-9A-Fa-f]+); // type:\w+"
)
NAMED = re.compile(r"(?:^fn_|_)([0-9A-Fa-f]{8})(?:_|$)")


def load_symbols():
    found = {}
    path = os.path.join(ROOT, "config", "symbols_all.txt")
    with open(path, "r", encoding="utf-8") as handle:
        for raw in handle:
            match = SYMBOL.match(raw.strip())
            if match:
                found[match.group(1)] = int(match.group(2), 16)
    return found


def address_for(name, symbols):
    if name in symbols:
        return symbols[name]
    match = NAMED.search(name)
    if match:
        return int(match.group(1), 16)
    return None


def scrub_measures(measures):
    if int(measures.get("total_data") or 0) == 0:
        for key in DATA_KEYS:
            measures.pop(key, None)
    if int(measures.get("total_code") or 0) == 0:
        for key in CODE_KEYS:
            measures.pop(key, None)


def walk(node, symbols):
    if isinstance(node, dict):
        measures = node.get("measures")
        if isinstance(measures, dict):
            scrub_measures(measures)
        if "size" in node and "name" in node and "address" in node:
            address = address_for(node["name"], symbols)
            if address is not None:
                metadata = node.setdefault("metadata", {})
                metadata["virtual_address"] = str(address)
        for value in node.values():
            walk(value, symbols)
    elif isinstance(node, list):
        for item in node:
            walk(item, symbols)


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "build", "report.json")
    with open(path, "r", encoding="utf-8") as handle:
        report = json.load(handle)
    walk(report, load_symbols())
    with open(path, "w", encoding="utf-8") as handle:
        json.dump(report, handle, indent=2)
        handle.write("\n")


if __name__ == "__main__":
    main()
