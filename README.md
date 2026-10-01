# Summoner

[![Build](https://github.com/AARosson48/SummonerDecomp/actions/workflows/check.yml/badge.svg)](https://github.com/AARosson48/SummonerDecomp/actions/workflows/check.yml)
[![Code](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc)
[![Data](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc.svg?mode=shield&measure=data&label=Data)](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc)

![Progress](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc.png?mode=report)

A matching decompilation of `Sum.exe`, the Volition engine from Summoner (PC, 2001).

Retail archives, movies, and audio stay in the Steam or GOG install. This repository has the reconstructed source, the headers, and the scripts that point a local build at that install. `binkw32.dll` and `eax.dll` are import stubs only.

The supported executable is version `sum-pc`:

- SHA1 `cdc5d71d447a89e9a1c5e176c65c9b129321d7fc`
- 1,785,856 bytes
- MSVC 6.0, COFF timestamp 2001-11-21
- The setup script accepts `Sum.exe` or `Summ.exe` when the bytes match

## Progress

decomp.dev scores **matched** bytes, from a rebuild that compares equal to `Sum.exe`. That score is 0 of 1,559,360 code bytes and 0 of 215,972 data bytes.

Code transcribed from the executable, still awaiting a byte compare, is 20,220 bytes, 1.30% of `.text`. The packfile reader, search paths, file object, buffered read, serializer, and text class are in that set. Data progress counts `.rdata` (37,412), the initialized `.data` (176,128), and `.rsrc` (2,432). The rest of `.data` is uninitialized storage and is not in the total.

The Code badge, the Data badge, and the progress graph update from the `sum-pc_report` artifact after the repository is public and added at [decomp.dev/manage/new](https://decomp.dev/manage/new). Use version id `sum-pc`. The [decomp.dev GitHub app](https://github.com/apps/decomp-dev) picks up the artifact when the Build workflow finishes.

## Setup

Python 3.10 or newer. On Windows, Developer Mode or an elevated shell is required for the symlinks.

```sh
python tools/extract_game.py
python configure.py
python tools/progress.py --report build/report.json
```

`extract_game.py` finds the install from the Steam uninstall key for app 2750, the GOG registry key, and every library listed in Steam's `libraryfolders.vdf`. A saved path from an earlier run is read from `project.local.json`, which is gitignored. If none of those locate `Sum.exe`, set `SUMMONER_DIR` to the install folder, or pass that folder with `--game-dir`.

The script copies only the executable into `baserom/baserom.exe`. It symlinks `*.vpp` and the runtime DLLs into `build/` and `assets/`. It stops if the executable hash is not `sum-pc`.

## What is in the executable

Assert strings in the executable still carry the source paths from the original build. The file list from those strings is in `config/modules.txt`. `config/symbols_all.txt` is every function in `.text`, named or not. Names we have recovered are in `config/symbols.txt`. Calls that still need a body are in `config/undefined_syms.txt`.

| Area | In Sum.exe |
| --- | --- |
| Compiler | Optional-header linker 6.0 |
| Archives | `chars`, `cutscene`, `effects`, `items`, `levele`, `levelm`, `levelt`, `music`, `sounds`, `summoner`, `tables` `.vpp` |
| Level types | `.pfg .mlo .vis .vex .bsp .lkf .eax` and `.v3d .s3d .vfx .vlm` |
| Renderer | `gr_direct3d.cpp`, `gr_opengl.cpp`, `gr_glide.cpp` |
| Multiplayer | `WSOCK32.dll` and PXO (`pxo.net`) |
| Middleware | `binkw32.dll`, `EAX.DLL` |

Matching follows the Windows projects that use [reccmp](https://github.com/isledecomp/reccmp). You compile this source with MSVC 6, link an executable, and reccmp compares each function in that executable with the same address in retail `Sum.exe`. `reccmp-project.yml` names that retail file and its hash. `config/reccmp.csv` is that full address list. A function with no name yet is still an address, written as `fn_` plus the address. `extract_game.py` writes `reccmp-user.yml` with the path to the `Sum.exe` on that machine, and that file is gitignored.

```sh
pip install reccmp
python configure.py
reccmp-reccmp --target SUM
```

`reccmp-reccmp` needs the MSVC 6 build and its PDB. Until that build exists, the command has nothing to compare, and the decomp.dev score stays at 0%.

## Later

Summoner and Red Faction share the Volition `vsdk` layout. [Glacier](https://github.com/GooberRF/glacier) is a Red Faction level editor and a reference for how that editor mounts a game install. Summoner levels in this executable are `.s3d`, `.pfg`, and `.lkf`. [Alpine Faction](https://github.com/GooberRF/alpinefaction) is the template for a modern client and level player once startup, packfiles, and level load match.
