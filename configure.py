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
    if lowered.startswith("bank/"):
        return "bank"
    if lowered.startswith("vsdk/"):
        return "vsdk"
    if lowered.startswith("levelscripts/"):
        return "scripts"
    if lowered.startswith("summoner/"):
        return "game"
    return "engine"


def msvc_root():
    """MSVC 6 tree. CI sets MSVC6_ROOT; a local tree defaults to C:\\projects\\MSVC600."""
    return os.path.normpath(os.environ.get("MSVC6_ROOT", r"C:\projects\MSVC600"))


def msvc_cl():
    return os.path.join(msvc_root(), "VC98", "Bin", "cl.exe")


def compiled_units():
    """Units whose source is compiled into an objdiff base object.

    Only files that already compile with MSVC 6. The rest of src/ stays out
    until each file compiles on its own. See notes/build.md.
    """
    if not os.path.isfile(msvc_cl()):
        return []
    # unit, source. level_archives_ready is 0x43E940 inside bank/0043B260.
    # The named Engine/s3d/s3d.cpp slice is a different 0x110-byte range.
    pairs = [
        ("bank/00401000", "src/bank/00401000.cpp"),
        ("bank/0043B260", "src/bank/0043B260.cpp"),
        ("bank/00512880", "src/bank/00512880.cpp"),
        ("vsdk/os/memory.cpp", "src/vsdk/os/memory.cpp"),
        ("bank/00524C10", "src/vsdk/vfile/vfs_file.cpp"),
        ("bank/00533620", "src/vsdk/vfile/packfile/file_packfile.cpp"),
        ("vsdk/vfile/packfile/file_packfile.cpp", "src/vsdk/vfile/packfile/file_packfile.cpp"),
        ("bank/005341A0", "src/vsdk/vfile/packfile/file_packfile.cpp"),
        ("bank/0054F790", "src/vsdk/vfile/vfs_file.cpp"),
        ("bank/00555CF0", "src/vsdk/os/text.cpp"),
        ("bank/00559B80", "src/vsdk/vfile/vfs_file.cpp"),
        ("vsdk/parse/parse.cpp", "src/vsdk/parse/parse.cpp"),
        ("levelscripts/script_internal.cpp", "src/levelscripts/script_internal.cpp"),
        ("bank/004D39C0", "src/bank/004D39C0.cpp"),
    ]
    return [(unit, src) for unit, src in pairs if os.path.isfile(os.path.join(ROOT, src))]


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
    compiled = {unit for unit, _src in compiled_units()}
    for name in split_units():
        metadata = {"complete": False}
        if name.endswith(".cpp") or name.startswith("bank/"):
            metadata["progress_categories"] = [category_for(name)]
        if name.endswith(".cpp"):
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
                "base_path": ("build/src/" + name + ".obj") if name in compiled else None,
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
            {"id": "bank", "name": "Banks"},
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
    compiled = compiled_units()
    compile_rules = ""
    compile_builds = ""
    report_inputs = "build/base/config.json"
    if os.name == "nt":
        report_command = (
            'cmd /s /c "tools\\bin\\objdiff-cli.exe report generate -p . -o build\\report.json -f json '
            '&& python tools\\scrub_report.py build\\report.json"'
        )
    else:
        report_command = (
            "tools/bin/objdiff-cli report generate -p . -o build/report.json -f json "
            "&& python tools/scrub_report.py build/report.json"
        )
    if compiled:
        root = msvc_root().replace("\\", "/")
        cl = root + "/VC98/Bin/cl.exe"
        include = root + "/VC98/Include"
        bin_dir = root + "/VC98/Bin"
        msdev = root + "/Common/MSDev98/Bin"
        # mkdir uses & so a folder that already exists does not skip cl.
        # PATH is only the two MSVC directories. cl loads MSPDB60.DLL from
        # MSDev98\\Bin (0xC0000135 without it). Do not append %PATH%: an &
        # in the machine PATH splits this command.
        compile_rules = f"""
rule cc
  command = cmd /s /c "for %I in ("$out") do mkdir "%~dpI" 2>nul & set INCLUDE={include}& set PATH={bin_dir};{msdev}& {cl} /nologo /O2 /c /I include /Fo"$out" $in"
  description = cl $in
"""
        lines = []
        for unit, src in compiled:
            obj = "build/src/" + unit + ".obj"
            lines.append(f"build {obj}: cc {src}")
        compile_builds = "\n".join(lines) + "\n"
        report_inputs += " " + " ".join("build/src/" + unit + ".obj" for unit, _src in compiled)
    text = f"""\
# Generated by configure.py.
# dtk splits orig/sum-pc/Sum.exe into one object per unit. objdiff compares
# those objects. Linking build/sum-pc/Sum.exe is the final sha1 check.
rule split
  command = tools/bin/dtk{exe} coff split --no-update config/dtk.yml build/base
  description = split Sum.exe
{compile_rules}
rule report
  command = {report_command}
  description = report

build build/base/config.json: split
{compile_builds}build build/report.json: report {report_inputs}
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
