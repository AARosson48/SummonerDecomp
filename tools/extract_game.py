#!/usr/bin/env python3
"""Point the workspace at a local Summoner install without copying assets.

Copies Sum.exe to retail_bin/Sum.exe and symlinks the VPP packages and
runtime DLLs into assets/ and build/. retail_bin/ is gitignored. The public
repository does not contain the retail executable.
"""

import argparse
import hashlib
import json
import os
import re
import shutil
import sys

try:
    import winreg
except ImportError:
    winreg = None

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
EXPECTED_SHA1 = "cdc5d71d447a89e9a1c5e176c65c9b129321d7fc"
LOCAL_CONFIG = os.path.join(ROOT, "project.local.json")
RUNTIME_FILES = (
    "binkw32.dll",
    "eax.dll",
    "Patchw32.dll",
    "Summoner.exe",
)


def _registry_candidates():
    if winreg is None:
        return
    keys = (
        (winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App 2750", "InstallLocation"),
        (winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Steam App 2750", "InstallLocation"),
        (winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\GOG.com\Games\1442823793", "path"),
    )
    for hive, subkey, value_name in keys:
        try:
            with winreg.OpenKey(hive, subkey) as key:
                path, _ = winreg.QueryValueEx(key, value_name)
        except OSError:
            continue
        if path:
            yield path
    try:
        with winreg.OpenKey(winreg.HKEY_CURRENT_USER, r"SOFTWARE\Valve\Steam") as key:
            steam, _ = winreg.QueryValueEx(key, "SteamPath")
    except OSError:
        steam = None
    if steam:
        yield os.path.join(steam, "steamapps", "common", "Summoner")


def _has_exe(path):
    return os.path.isfile(os.path.join(path, "Sum.exe")) or os.path.isfile(os.path.join(path, "Summ.exe"))


def _steam_libraries():
    roots = []
    if winreg is not None:
        try:
            with winreg.OpenKey(winreg.HKEY_CURRENT_USER, r"SOFTWARE\Valve\Steam") as key:
                steam, _ = winreg.QueryValueEx(key, "SteamPath")
        except OSError:
            steam = None
        if steam:
            roots.append(steam)
    for env_name in ("ProgramFiles(x86)", "ProgramFiles"):
        base = os.environ.get(env_name)
        if base:
            roots.append(os.path.join(base, "Steam"))
    seen = set()
    for root in roots:
        if not root or root in seen:
            continue
        seen.add(root)
        yield os.path.join(root, "steamapps", "common", "Summoner")
        for rel in ("steamapps/libraryfolders.vdf", "config/libraryfolders.vdf"):
            vdf_path = os.path.join(root, rel)
            if not os.path.isfile(vdf_path):
                continue
            with open(vdf_path, "r", encoding="utf-8", errors="replace") as handle:
                text = handle.read()
            for match in re.finditer(r'"path"\s+"([^"]+)"', text):
                library = match.group(1).replace("\\\\", "\\")
                yield os.path.join(library, "steamapps", "common", "Summoner")


def find_game_dir(explicit):
    if explicit:
        return explicit
    if os.environ.get("SUMMONER_DIR"):
        return os.environ["SUMMONER_DIR"]
    if os.path.isfile(LOCAL_CONFIG):
        with open(LOCAL_CONFIG, "r", encoding="utf-8") as handle:
            saved = json.load(handle).get("game_dir")
        if saved and _has_exe(saved):
            return saved
    for path in list(_registry_candidates()) + list(_steam_libraries()):
        if path and _has_exe(path):
            return path
    return None


def _sha1(path):
    digest = hashlib.sha1()
    with open(path, "rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _link(src, dst):
    if os.path.lexists(dst):
        return False
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    os.symlink(src, dst, target_is_directory=os.path.isdir(src))
    return True


def setup(game_dir):
    game_dir = os.path.abspath(game_dir)
    if not _has_exe(game_dir):
        sys.exit(f"No Sum.exe or Summ.exe in {game_dir}")

    exe_name = "Sum.exe" if os.path.isfile(os.path.join(game_dir, "Sum.exe")) else "Summ.exe"
    src_exe = os.path.join(game_dir, exe_name)
    digest = _sha1(src_exe)
    if digest != EXPECTED_SHA1:
        sys.exit(
            f"{exe_name} sha1 {digest} does not match the pinned sum-pc revision {EXPECTED_SHA1}"
        )

    baserom = os.path.join(ROOT, "baserom")
    build = os.path.join(ROOT, "build")
    assets = os.path.join(ROOT, "assets")
    os.makedirs(baserom, exist_ok=True)
    os.makedirs(build, exist_ok=True)
    os.makedirs(assets, exist_ok=True)

    retail = os.path.join(ROOT, "retail_bin")
    os.makedirs(retail, exist_ok=True)
    shutil.copy2(src_exe, os.path.join(retail, "Sum.exe"))
    print(f"isolated {exe_name} -> retail_bin/Sum.exe ({digest})")
    shutil.copy2(src_exe, os.path.join(baserom, "baserom.exe"))

    names = [name for name in os.listdir(game_dir) if name.lower().endswith(".vpp")]
    names.extend(name for name in RUNTIME_FILES if os.path.isfile(os.path.join(game_dir, name)))
    for name in names:
        src = os.path.join(game_dir, name)
        try:
            if _link(src, os.path.join(build, name)):
                print(f"linked build/{name}")
            if name.lower().endswith(".vpp") and _link(src, os.path.join(assets, name)):
                print(f"linked assets/{name}")
        except OSError as exc:
            print(f"could not link {name}: {exc}")
            print("Windows symlinks need Developer Mode or an elevated shell.")

    with open(LOCAL_CONFIG, "w", encoding="utf-8") as handle:
        json.dump({"game_dir": game_dir}, handle, indent=2)
        handle.write("\n")
    print("game_dir saved to project.local.json")

    user_yml = os.path.join(ROOT, "reccmp-user.yml")
    with open(user_yml, "w", encoding="utf-8", newline="\n") as handle:
        handle.write("targets:\n")
        handle.write("  SUM:\n")
        handle.write('    path: "retail_bin/Sum.exe"\n')
    print("original executable recorded in reccmp-user.yml")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--game-dir", help="Summoner install directory")
    args = parser.parse_args()
    game_dir = find_game_dir(args.game_dir)
    if not game_dir:
        sys.exit("Could not find Summoner. Pass --game-dir or set SUMMONER_DIR.")
    setup(game_dir)


if __name__ == "__main__":
    main()
