# Summoner

[![Build](https://github.com/AARosson48/SummonerDecomp/actions/workflows/check.yml/badge.svg)](https://github.com/AARosson48/SummonerDecomp/actions/workflows/check.yml)
[![Code](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc)
[![Data](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc.svg?mode=shield&measure=data&label=Data)](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc)

![Progress](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc.png?mode=report)

A matching decompilation of `Sum.exe`, the PC executable of Summoner (2001).

The supported build is SHA1 `cdc5d71d447a89e9a1c5e176c65c9b129321d7fc` (1,785,856 bytes, 2001-11-21). We compile with MSVC 6 `cl` 12.00.8804 `/O2` from [itsmattkc/MSVC600](https://github.com/itsmattkc/MSVC600). The shields and the graph above are the live score on [decomp.dev](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc). Matched code counts functions objdiff scores at 100%.

Most of the C in `src/` was drafted with Cursor's coding agent and checked with objdiff. Details are in [docs/matching.md](docs/matching.md).

Retail archives stay in your Steam or GOG install. `Sum.exe` is not in this repository.

## Get started

Python 3.10, [Ninja](https://ninja-build.org/), a checkout of MSVC600, and your own copy of the game. `configure.py` looks for `cl.exe` at `C:\projects\MSVC600` unless `MSVC6_ROOT` is set. `tools/bin/` needs decomp-toolkit 0.0.27 (`dtk.exe`) and objdiff 3.8.2 (`objdiff-cli.exe`). CI downloads those two.

```sh
python tools/extract_game.py
python configure.py
ninja
```

`extract_game.py` finds the install from Steam app 2750, the GOG registry key, or `libraryfolders.vdf`. If it does not, set `SUMMONER_DIR` or pass `--game-dir`. It copies `Sum.exe` to `orig/sum-pc/Sum.exe`.

`ninja` splits that executable, compiles the files listed in `configure.py` `compiled_units()`, and writes `build/report.json`.

To work on a function, see [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md).
