#!/usr/bin/env python3
"""Map every VPP/tables/*.tbl header to the Sum.exe function that references it.

Reads each table, finds those header strings in the executable, and writes
notes/tbl_map.md plus notes/tbl_map.json. The headers on a function are the
record it reads. Read that map before writing C for the function. This script
does not assign a match score.
"""

import argparse
import json
import os
import re
import struct

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
HEADER_RE = re.compile(r"^([$#+][A-Za-z][A-Za-z0-9_ ]{0,60})")
FUNC_RE = re.compile(
    r"(\S+) = \.text:0x([0-9A-Fa-f]+); // type:function(?: size:0x([0-9A-Fa-f]+))?"
)


def exe_path():
    for rel in ("orig/sum-pc/Sum.exe", "baserom/baserom.exe"):
        path = os.path.join(ROOT, rel)
        if os.path.isfile(path):
            return path
    raise SystemExit("Sum.exe not found at orig/sum-pc/Sum.exe")


def pe_sections(data):
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    nsec = struct.unpack_from("<H", data, pe + 6)[0]
    soh = struct.unpack_from("<H", data, pe + 20)[0]
    sections = []
    off = pe + 24 + soh
    for i in range(nsec):
        entry = off + i * 40
        name = data[entry : entry + 8].split(b"\0", 1)[0].decode("ascii", "replace")
        vsize, va, rawsize, rawptr = struct.unpack_from("<IIII", data, entry + 8)
        sections.append(
            {"name": name, "va": va, "vsize": vsize, "rawptr": rawptr, "rawsize": rawsize}
        )
    return sections


def cstrings(blob, va_base):
    i = 0
    n = len(blob)
    while i < n:
        if 32 <= blob[i] < 127:
            j = i
            while j < n and 32 <= blob[j] < 127:
                j += 1
            if j < n and blob[j] == 0 and j - i >= 2:
                yield va_base + i, blob[i:j].decode("ascii")
            i = j + 1
        else:
            i += 1


def load_functions():
    funcs = []
    path = os.path.join(ROOT, "config", "dtk_symbols.txt")
    with open(path, "r", encoding="utf-8") as handle:
        for line in handle:
            match = FUNC_RE.match(line)
            if not match or not match.group(3):
                continue
            start = int(match.group(2), 16)
            size = int(match.group(3), 16)
            funcs.append(
                {"name": match.group(1), "start": start, "end": start + size, "size": size}
            )
    funcs.sort(key=lambda item: item["start"])
    return funcs


def load_units():
    units = []
    name = None
    path = os.path.join(ROOT, "config", "splits.txt")
    with open(path, "r", encoding="utf-8") as handle:
        for raw in handle:
            line = raw.strip()
            if line.endswith(":") and not line.startswith(".") and not line.startswith("#"):
                name = line[:-1]
                continue
            match = re.search(r"start:0x([0-9A-Fa-f]+) end:0x([0-9A-Fa-f]+)", line)
            if match and name and ".text" in line:
                units.append((int(match.group(1), 16), int(match.group(2), 16), name))
    return units


def owner(funcs, addr):
    for func in funcs:
        if func["start"] <= addr < func["end"]:
            return func
    return None


def unit_of(units, addr):
    for start, end, name in units:
        if start <= addr < end:
            return name
    return ""


def read_tables(folder):
    """header -> set of tbl filenames, and filename -> set of headers."""
    by_header = {}
    by_file = {}
    empty = []
    for dirpath, _dirs, files in os.walk(folder):
        for filename in files:
            if not filename.lower().endswith(".tbl"):
                continue
            path = os.path.join(dirpath, filename)
            headers = set()
            with open(path, "r", encoding="latin1", errors="replace") as handle:
                for line in handle:
                    match = HEADER_RE.match(line.strip())
                    if match:
                        headers.add(match.group(1).rstrip())
            if not headers:
                empty.append(filename)
                continue
            by_file[filename] = headers
            for header in headers:
                by_header.setdefault(header, set()).add(filename)
    return by_header, by_file, empty


def match_header(text, headers_long_first):
    for header in headers_long_first:
        if text == header or text.startswith(header + ":") or text.startswith(header + "\t"):
            return header
    return None


def build(exe, tables_dir):
    data = open(exe, "rb").read()
    sections = pe_sections(data)
    text = next(sec for sec in sections if sec["name"] == ".text")
    text_bytes = data[text["rawptr"] : text["rawptr"] + text["rawsize"]]
    text_va = 0x400000 + text["va"]
    by_header, by_file, empty = read_tables(tables_dir)
    headers_long_first = sorted(by_header, key=len, reverse=True)

    # Every copy of a header string in the image, then the code that points at it.
    header_vas = {}
    for sec in sections:
        if sec["name"] not in (".rdata", ".data"):
            continue
        blob = data[sec["rawptr"] : sec["rawptr"] + sec["rawsize"]]
        base = 0x400000 + sec["va"]
        for va, text_s in cstrings(blob, base):
            header = match_header(text_s, headers_long_first)
            if header:
                header_vas.setdefault(header, set()).add(va)

    funcs = load_functions()
    units = load_units()
    func_headers = {}
    for header, vas in header_vas.items():
        for va in vas:
            pat = struct.pack("<I", va)
            pos = 0
            while True:
                found = text_bytes.find(pat, pos)
                if found < 0:
                    break
                func = owner(funcs, text_va + found)
                if func:
                    slot = func_headers.setdefault(
                        func["name"],
                        {
                            "name": func["name"],
                            "address": func["start"],
                            "size": func["size"],
                            "unit": unit_of(units, func["start"]),
                            "headers": set(),
                        },
                    )
                    slot["headers"].add(header)
                pos = found + 1

    by_start = {func["address"]: func["name"] for func in func_headers.values()}
    for func in func_headers.values():
        func["calls"] = calls_in(data, func, by_start)

    return {
        "by_header": by_header,
        "by_file": by_file,
        "empty": empty,
        "header_vas": header_vas,
        "func_headers": func_headers,
    }


def calls_in(data, func, by_start):
    """Other mapped functions this function calls with a direct rel32 call."""
    start = func["address"] - 0x400000
    blob = data[start : start + func["size"]]
    found = set()
    i = 0
    while i < len(blob) - 4:
        if blob[i] == 0xE8:
            rel = struct.unpack_from("<i", blob, i + 1)[0]
            target = func["address"] + i + 5 + rel
            name = by_start.get(target)
            if name and name != func["name"]:
                found.add(name)
            i += 5
            continue
        i += 1
    return sorted(found)


# A reading of the record. The address stays fn_. These sentences are the
# guess the headers and the table filenames support.
READINGS = {
    "fn_004452F0": "Character stat block. The closest files are monster and NPC tables such as Golem.tbl and Jekhar.tbl.",
    "fn_004FEE50": "Dialogue action list. The closest files are *_dlg.tbl. The keys grant or remove items, XP, gold, topics, and quest visibility.",
    "fn_0047A190": "Spell list. spells.tbl contains every header this function references.",
    "fn_004CD5F0": "Level layout script: doors, navpoints, objects, town, and clicks. The closest files are *_script.tbl.",
    "fn_004355E0": "Cutscene table. cutscene.tbl contains every header: camera, animation, fade, fog, soundtrack, and script.",
    "fn_004CB7B0": "Level atmosphere. masad.tbl contains every header: fog, ambient sounds, shopkeepers, effects, and automap.",
    "fn_00500065": "Dialogue topic list. Same bank as the dialogue action reader. Miniquests_dlg.tbl is the closest file.",
    "fn_004DBA30": "Spawn list. Keys are monster, boss, miniboss, character, team, and trigger. The closest files are *_script.tbl.",
    "fn_004CC190": "Trigger volume. Keys are plane, radius, script, and spline. The closest files are level tables.",
    "fn_004CB430": "Level item list. level_items.tbl contains every header.",
    "fn_004D39C0": "Quest list and per-level event flags. events.tbl and demo_events.tbl contain every header. A quest is $Quest, +Main quest, and a series of +Stage lines. A level block is $Level and +Flag.",
    "fn_00449F60": "Material modifiers. materials.tbl contains every header.",
    "fn_004ED0F0": "Cutscene navpoint count. The closest files are *_cscript.tbl.",
    "fn_004D6100": "Which script and dialogue file a level loads. script_filenames.tbl contains every header. Same bank as the quest list.",
    "fn_004DB420": "Cutscene navpoint record: $Name, $Type, +Plane, $Position, $Orientation. The tables are *_cscript.tbl. Compiled C for this function is already on the objdiff compare.",
    "fn_004DB700": "Second reader of that same cutscene navpoint record. Same 210 *_cscript.tbl files. No compiled C yet.",
    "fn_00410760": "Menu cameras. The tables are bs_cameras.tbl, inv_cameras.tbl, and status_cameras.tbl.",
    "fn_004691E0": "Sound or music entry. The closest files are feedback.tbl and music.tbl. The unit is gamesound.cpp.",
    "fn_004874C0": "Shadow list. shadows.tbl contains every header.",
    "fn_004CC4D0": "Shopkeeper buy and sell lists. The tables are level files such as IonaExt.tbl and Wolong.tbl.",
    "fn_00437A40": "Town character list. The tables are *_script.tbl.",
    "fn_004AAC10": "Alternate textures. alt_textures.tbl contains every header.",
    "fn_004D4020": "Automap images for a level. Same bank as the quest list.",
    "fn_004D4160": "Level item names. level_items.tbl. Same bank as the quest list.",
    "fn_00504520": "Localized string block. The header is #english. The closest files include credits, item names, quest names, and strings.",
    "fn_0042FD40": "$Type on the cutscene scripts. Same *_cscript.tbl set as the navpoint readers.",
    "fn_00446930": "Character resistances. The tables are the character .tbl files. Same unit as the character stat block.",
    "fn_00468E30": "Surface defaults. The tables are *_surfaces.tbl.",
    "fn_00504870": "Named script and topic file list. script_names.tbl and topic_names.tbl. The keys are #Stop File and #english.",
    "fn_00521A60": "Sound list bounds. sounds.tbl contains both headers.",
    "fn_00408590": "String or credits block. The only header is #end. The tables are credits.tbl and strings.tbl.",
    "fn_00435FF0": "Cutscene visibility set. The only header is $VisSet, and the only table is cutscene.tbl. Same bank as the cutscene table reader.",
    "fn_00446790": "Character sound. The only header is $Sound, on the character .tbl files. Same unit as the character stat block.",
    "fn_00454D41": "Poison feedback. The only header is +Poison, and the only table is feedback.tbl.",
    "fn_0047A6C0": "A $Sound reader in the spell bank, next to the spell list. The tables are the character .tbl files.",
    "fn_004CC010": "Level ambient sound. The only header is $Ambient, on the level .tbl files. Same bank as the door, trigger, and shopkeeper readers.",
    "fn_00504230": "String or credits block. The only header is #end. Same bank as the localized string reader.",
}


def tables_for(func_headers, by_file):
    """Tables that contain this function's whole header set, then partials."""
    needed = func_headers
    full = []
    partial = []
    for filename, headers in by_file.items():
        hit = len(needed & headers)
        if hit == 0:
            continue
        if hit == len(needed):
            full.append(filename)
        else:
            partial.append((hit, filename))
    full.sort()
    partial.sort(key=lambda item: (-item[0], item[1]))
    return full, partial


def best_function(file_headers, func_headers):
    best = None
    for func in func_headers.values():
        hit = len(file_headers & func["headers"])
        if hit == 0:
            continue
        ratio = hit / len(func["headers"])
        rank = (hit, ratio, -len(func["headers"]))
        if best is None or rank > best[0]:
            best = (rank, func["name"], hit, len(func["headers"]))
    return best


def reading_for(name):
    if name in READINGS:
        return READINGS[name]
    if name.startswith("_") and name[1:] in READINGS:
        return READINGS[name[1:]]
    return ""


def call_groups(func_headers):
    """Connected components of the direct-call graph, largest first."""
    names = sorted(func_headers)
    parent = {name: name for name in names}

    def find(name):
        while parent[name] != name:
            parent[name] = parent[parent[name]]
            name = parent[name]
        return name

    def union(left, right):
        left = find(left)
        right = find(right)
        if left != right:
            parent[right] = left

    for func in func_headers.values():
        for callee in func.get("calls") or []:
            if callee in parent:
                union(func["name"], callee)
    groups = {}
    for name in names:
        groups.setdefault(find(name), []).append(name)
    ordered = [sorted(group) for group in groups.values() if len(group) > 1]
    ordered.sort(key=lambda group: (-len(group), group[0]))
    return ordered


def write_md(path, result, exe):
    by_header = result["by_header"]
    by_file = result["by_file"]
    header_vas = result["header_vas"]
    func_headers = result["func_headers"]
    present = [h for h in by_header if h in header_vas]
    absent = sorted(h for h in by_header if h not in header_vas)
    lines = []
    add = lines.append
    add("# Table reference map")
    add("")
    add("Generated by `python tools/tbl_map.py`.")
    add("")
    add("The script reads every `VPP/tables/*.tbl`. A header is a line that starts with `$`, `#`, or `+`. It then finds those strings in `Sum.exe` and records the function that references each string.")
    add("")
    add("Read this before writing C for a function. The headers are the record that function looks up. The tables are the files that contain that record. The Reading line is the guess those headers and filenames support. Calls are direct calls to other functions on this map. The address stays `fn_` until a string in the binary proves a name. objdiff scores the C after it compiles. This file does not assign a match percent.")
    add("")
    add("Regenerate this file when `VPP/tables` or `config/dtk_symbols.txt` changes.")
    add("")
    add("## Coverage")
    add("")
    add("- Executable: `%s`" % os.path.relpath(exe, ROOT).replace("\\", "/"))
    add("- Tables read: %d" % len(by_file))
    add("- Tables with no `$` / `#` / `+` header: %d" % len(result["empty"]))
    add("- Distinct headers: %d" % len(by_header))
    add("- Headers found as strings in the executable: %d" % len(present))
    add("- Headers that occur only in the tables: %d" % len(absent))
    add("- Functions that reference at least one header: %d" % len(func_headers))
    add("")
    add("A header that occurs only in the tables is a value the code builds, or a key spelled differently in the executable.")
    add("")
    add("## Functions")
    add("")
    add("Sorted by how many distinct headers the function references. A longer shared set is a stronger guess at the record.")
    add("")
    ranked = sorted(
        func_headers.values(),
        key=lambda item: (-len(item["headers"]), item["address"]),
    )
    for func in ranked:
        headers = sorted(func["headers"])
        full, partial = tables_for(set(headers), by_file)
        add("### `%s`" % func["name"])
        add("")
        add("- Address: `0x%08X` size `0x%X`" % (func["address"], func["size"]))
        if func["unit"]:
            add("- Unit: `%s`" % func["unit"])
        add("- Headers (%d): %s" % (len(headers), ", ".join("`%s`" % h for h in headers)))
        reading = reading_for(func["name"])
        if reading:
            add("- Reading: %s" % reading)
        calls = func.get("calls") or []
        if calls:
            add("- Calls: %s" % ", ".join("`%s`" % name for name in calls))
        siblings = [
            other["name"]
            for other in func_headers.values()
            if other["unit"] and other["unit"] == func["unit"] and other["name"] != func["name"]
        ]
        if siblings:
            add("- Same unit: %s" % ", ".join("`%s`" % name for name in sorted(siblings)))
        if full:
            shown = full[:12]
            add("- Tables that contain this whole set: %d" % len(full))
            for name in shown:
                add("  - `%s`" % name)
            if len(full) > len(shown):
                add("  - and %d more" % (len(full) - len(shown)))
        elif partial:
            add("- No table contains every header. Closest tables:")
            for hit, name in partial[:8]:
                add("  - `%s` (%d of %d)" % (name, hit, len(headers)))
        add("")

    add("## Call groups")
    add("")
    add("Functions in one group call each other, directly, among the functions on this map. A reading that mentions the same record belongs with that group.")
    add("")
    for group in call_groups(func_headers):
        names = ", ".join("`%s`" % name for name in group)
        add("- %s" % names)
        for name in group:
            reading = reading_for(name)
            if reading:
                add("  - `%s`: %s" % (name, reading))
    add("")

    add("## Tables")
    add("")
    add("Every `.tbl` file. Overlap is how many of that file's headers the best function also references, out of the function's own header count.")
    add("")
    add("| Table | Headers in file | Best function | Overlap |")
    add("| --- | --- | --- | --- |")
    for filename in sorted(by_file):
        headers = by_file[filename]
        best = best_function(headers, func_headers)
        if best is None:
            add("| `%s` | %d |  |  |" % (filename, len(headers)))
            continue
        _rank, name, hit, total = best
        add("| `%s` | %d | `%s` | %d/%d |" % (filename, len(headers), name, hit, total))
    add("")
    if absent:
        add("## Headers present only in the tables")
        add("")
        for header in absent:
            files = sorted(by_header[header])
            sample = ", ".join("`%s`" % name for name in files[:4])
            more = "" if len(files) <= 4 else " and %d more" % (len(files) - 4)
            add("- `%s` in %s%s" % (header, sample, more))
        add("")
    text = "\n".join(lines)
    with open(path, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(text)


def write_json(path, result):
    by_file = result["by_file"]
    func_headers = result["func_headers"]
    functions = {}
    for func in func_headers.values():
        headers = sorted(func["headers"])
        full, partial = tables_for(set(headers), by_file)
        functions[func["name"]] = {
            "address": "0x%08X" % func["address"],
            "size": "0x%X" % func["size"],
            "unit": func["unit"],
            "headers": headers,
            "reading": reading_for(func["name"]),
            "calls": func.get("calls") or [],
            "tables_full": full,
            "tables_partial": [
                {"file": name, "headers": hit} for hit, name in partial[:20]
            ],
        }
    tables = {}
    for filename in sorted(by_file):
        headers = by_file[filename]
        best = best_function(headers, func_headers)
        entry = {
            "headers": sorted(headers),
            "best_function": None,
            "overlap": 0,
            "function_headers": 0,
        }
        if best is not None:
            _rank, name, hit, total = best
            entry["best_function"] = name
            entry["overlap"] = hit
            entry["function_headers"] = total
        tables[filename] = entry
    payload = {
        "tables_read": len(by_file),
        "headers": len(result["by_header"]),
        "headers_in_exe": len(result["header_vas"]),
        "functions": functions,
        "tables": tables,
    }
    with open(path, "w", encoding="utf-8", newline="\n") as handle:
        json.dump(payload, handle, indent=1)
        handle.write("\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--tables",
        default=os.path.join(ROOT, "VPP", "tables"),
        help="folder of extracted .tbl files",
    )
    parser.add_argument(
        "--md",
        default=os.path.join(ROOT, "notes", "tbl_map.md"),
        help="markdown map to write",
    )
    parser.add_argument(
        "--json",
        default=os.path.join(ROOT, "notes", "tbl_map.json"),
        help="json map to write",
    )
    args = parser.parse_args()
    exe = exe_path()
    result = build(exe, args.tables)
    write_md(args.md, result, exe)
    write_json(args.json, result)
    print(
        "tables %d  headers %d  in exe %d  functions %d"
        % (
            len(result["by_file"]),
            len(result["by_header"]),
            len(result["header_vas"]),
            len(result["func_headers"]),
        )
    )
    print(os.path.relpath(args.md, ROOT))
    print(os.path.relpath(args.json, ROOT))


if __name__ == "__main__":
    main()
