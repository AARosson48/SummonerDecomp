#!/usr/bin/env python3
"""Compile this tree with MSVC 6 and link build/Sum.exe.

The retail comparison target is retail_bin/Sum.exe. This script only writes
the rebuilt binary and its PDB.
"""

import os
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))


def compiler_paths():
    root = os.environ.get("MSVC6", r"C:\projects\MSVC600")
    bin_dir = os.path.join(root, "VC98", "Bin")
    cl = os.path.join(bin_dir, "cl.exe")
    link = os.path.join(bin_dir, "link.exe")
    if not os.path.isfile(cl) or not os.path.isfile(link):
        sys.exit(f"MSVC 6 cl.exe not found at {cl}")
    env = os.environ.copy()
    env["PATH"] = os.pathsep.join(
        [
            bin_dir,
            os.path.join(root, "Common", "MSDev98", "Bin"),
            env.get("PATH", ""),
        ]
    )
    env["INCLUDE"] = os.path.join(root, "VC98", "Include")
    env["LIB"] = os.path.join(root, "VC98", "Lib")
    return cl, link, env


def sources():
    found = []
    src = os.path.join(ROOT, "src")
    for dirpath, _, filenames in os.walk(src):
        for name in filenames:
            if name.endswith(".cpp"):
                found.append(os.path.join(dirpath, name))
    return found


def main():
    cl, link, env = compiler_paths()
    cpp_files = sources()
    if not cpp_files:
        sys.exit("no .cpp files under src/")

    obj_dir = os.path.join(ROOT, "build", "obj")
    os.makedirs(obj_dir, exist_ok=True)
    objects = []
    for path in cpp_files:
        rel = os.path.relpath(path, os.path.join(ROOT, "src"))
        leaf = rel.replace("\\", "_").replace("/", "_")
        if leaf.endswith(".cpp"):
            leaf = leaf[:-4]
        obj = os.path.join(obj_dir, leaf + ".obj")
        print(f"cl /O2 /Zi /c {rel}")
        result = subprocess.run(
            [cl, "/nologo", "/O2", "/Zi", "/c", "/Fo" + obj, path],
            cwd=ROOT,
            env=env,
        )
        if result.returncode != 0:
            sys.exit(result.returncode)
        objects.append(obj)

    out = os.path.join(ROOT, "build", "Sum.exe")
    pdb = os.path.join(ROOT, "build", "Sum.pdb")
    command = [link, "/nologo", "/DEBUG", "/OUT:" + out, "/PDB:" + pdb] + objects
    print("link /OUT:build/Sum.exe")
    result = subprocess.run(command, cwd=ROOT, env=env)
    sys.exit(result.returncode)


if __name__ == "__main__":
    main()
