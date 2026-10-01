#!/usr/bin/env python3
"""Write build.ninja and objdiff.json for the Summoner matching decomp."""

import json
import os

ROOT = os.path.dirname(os.path.abspath(__file__))


def modules():
    path = os.path.join(ROOT, "config", "modules.txt")
    found = []
    with open(path, "r", encoding="utf-8") as handle:
        for line in handle:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            rel = line.split("pccode\\", 1)[-1].replace("\\", "/")
            found.append(rel)
    return found


def category_for(rel):
    lowered = rel.lower()
    if lowered.startswith("vsdk/"):
        return "vsdk"
    if lowered.startswith("levelscripts/"):
        return "scripts"
    if lowered.startswith("summoner/"):
        return "game"
    return "engine"


def split_units():
    """Unit names from config/splits.txt, in link order."""
    path = os.path.join(ROOT, "config", "splits.txt")
    names = []
    with open(path, "r", encoding="utf-8") as handle:
        for raw in handle:
            line = raw.strip()
            if not line or line.startswith("#") or line.startswith("Sections"):
                continue
            if line.endswith(":") and not line.startswith("."):
                names.append(line[:-1])
    return names


def write_objdiff():
    # One objdiff unit per splits.txt object. objdiff schedules report
    # work per unit, so the banks are what keep the other cores busy.
    units = []
    for name in split_units():
        metadata = {"complete": False}
        if name.endswith(".cpp"):
            metadata["progress_categories"] = [category_for(name)]
            stem = name[:-4]
        else:
            stem = name
        directory, leaf = os.path.split(stem)
        # dtk doubles underscores when it writes the object file name.
        leaf = leaf.replace("_", "__")
        obj_name = "/".join(part for part in (directory.replace("\\", "/"), leaf) if part) + ".o"
        units.append(
            {
                "name": name,
                "target_path": "build/base/obj/" + obj_name,
                "base_path": None,
                "metadata": metadata,
            }
        )
    payload = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "ninja",
        "build_target": False,
        "build_base": False,
        "watch_patterns": ["*.c", "*.cpp", "*.h", "*.py", "*.yml", "*.yaml", "*.txt", "*.json"],
        "ignore_patterns": ["build/**/*", "VPP/**/*", "baserom/**/*", "orig/**/*", "retail_bin/**/*"],
        "progress_categories": [
            {"id": "engine", "name": "Engine"},
            {"id": "vsdk", "name": "Volition SDK"},
            {"id": "game", "name": "Game"},
            {"id": "scripts", "name": "Level scripts"},
        ],
        "units": units,
    }
    with open(os.path.join(ROOT, "objdiff.json"), "w", encoding="utf-8") as handle:
        json.dump(payload, handle, indent=2)
        handle.write("\n")


def _symbol_rows(path):
    rows = []
    with open(path, "r", encoding="utf-8") as handle:
        for raw in handle:
            line = raw.split("#", 1)[0].strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) < 3:
                continue
            rows.append((parts[0], parts[1], parts[2]))
    return rows


def _entity_type(name):
    if name.startswith("crt_") or name in {
        "MoveFileA",
        "DeleteFileA",
        "GetLastError",
        "GetFileAttributesA",
        "GetCurrentDirectoryA",
        "SetFileAttributesA",
    }:
        return "library"
    if name.startswith("g_"):
        return "global"
    return "function"


def write_reccmp_csv():
    """Address list in the form reccmp reads.

    Every function from config/symbols_all.txt is included. That file is the
    decomp-toolkit scan of Sum.exe plus the names already in symbols.txt.
    """
    import re

    pattern = re.compile(
        r"(\S+) = \.\w+:0x([0-9A-Fa-f]+); // type:(\w+)(?: size:0x([0-9A-Fa-f]+))?"
    )
    path = os.path.join(ROOT, "config", "symbols_all.txt")
    rows = []
    with open(path, "r", encoding="utf-8") as handle:
        for raw in handle:
            match = pattern.match(raw.strip())
            if not match:
                continue
            name, address, kind, size = match.groups()
            if kind == "function":
                csv_type = "function"
            elif kind == "object":
                csv_type = "global"
            else:
                csv_type = "function"
            size_cell = f"0x{size}" if size else ""
            rows.append((address, name, csv_type, size_cell))
    destination = os.path.join(ROOT, "config", "reccmp.csv")
    with open(destination, "w", encoding="utf-8", newline="\n") as handle:
        handle.write("address|name|type|size\n")
        for address, name, csv_type, size_cell in rows:
            handle.write(f"0x{address}|{name}|{csv_type}|{size_cell}\n")
    return len(rows)


def write_ninja():
    exe = ".exe" if os.name == "nt" else ""
    text = f"""\
# Generated by configure.py.
# dtk splits orig/sum-pc/Sum.exe into one object per unit. objdiff compares
# those objects. Linking build/sum-pc/Sum.exe is the final sha1 check.
rule split
  command = tools/bin/dtk{exe} coff split --no-update config/dtk.yml build/base
  description = split Sum.exe

rule report
  command = tools/bin/objdiff-cli{exe} report generate -p . -o build/report.json -f json
  description = report

build build/base/config.json: split
build build/report.json: report build/base/config.json
default build/report.json
"""
    with open(os.path.join(ROOT, "build.ninja"), "w", encoding="utf-8", newline="\n") as handle:
        handle.write(text)


def main():
    write_objdiff()
    write_ninja()
    from tools.build_symbols import write_symbols

    text_count, _covered = write_symbols()
    marked = write_reccmp_csv()
    print(
        f"wrote objdiff.json ({len(split_units())} units), build.ninja, "
        f"config/symbols_all.txt ({text_count} .text symbols), "
        f"and config/reccmp.csv ({marked} addresses)"
    )


if __name__ == "__main__":
    main()
