# Summoner PC decomp

Read `notes/build.md` before changing the build, `configure.py`, or the decomp.dev report. That file is the record of what the toolkit source says and what has actually been run. Do not invent a second procedure.

## Summoner facts that differ

- Target name is `SUM`. Binary is `Sum.exe`. SHA1 `cdc5d71d447a89e9a1c5e176c65c9b129321d7fc`.
- Compiler recorded in the Rich header: Utc12_CPP build 8966, linker 6.00.8447. The portable tree at `C:\projects\MSVC600` (`itsmattkc/MSVC600`) prints `cl` 12.00.8804 and `dumpbin` 6.00.8447. Racers uses `cl` 12.00.8168. Do not copy that version.
- Original source paths are the assert strings in `config/modules.txt` (`D:\projects\Summoner\pccode\...`). A function goes in that `.cpp` once an assert, string cluster, or address range places it there.
- `// FUNCTION: SUM 0x........` means a 100% reccmp match. Anything short of that stays `// STUB:`. Unknown names stay `FUN_xxxxxxxx` until a match or a string proves a name.
- The original is `orig/sum-pc/Sum.exe`. The finished link is `build/sum-pc/Sum.exe`, checked by `config/sum-pc/build.sha1`. `tools/progress.py` does not decide a match.
- `Sum.exe` is not in this repository. The private repo `AARosson48/SummonerDecomp-retail` holds it. CI checks that repo out into `orig/sum-pc/` with `RETAIL_BINARY_TOKEN`. `dtk coff split` writes one object per unit plus `build/base/args.rsp` and `build/base/objs.rsp`. The link command for those files is `lld-link`, documented in decomp-toolkit `src/util/rsp.rs`. MSVC 6 `link.exe` is not that linker. See `notes/build.md`.

## Build and diff pipeline

Declare one state before acting on a build result. If the build log or objdiff output is missing, ask for it.

- **[STATE 1] BUILD FAILURE.** MSVC returns non-zero or reports a syntax error. Fix syntax only. Leave the function's logic and structure as they are. Rebuild.
- **[STATE 2] DIFF MISMATCH.** The compile succeeded and objdiff reports different instructions, stack, or registers. This is a matching error. Change types, alignment, or calling convention so MSVC emits the original bytes. Rebuild.
- **[STATE 3] BINARY MATCH.** objdiff reports a 1:1 match for that function. Record the decomp.dev entry from that objdiff output, including the MSVC version and flags that produced it.

A function stays unmatched until objdiff verifies the bytes.

## What this repo currently does wrong

- `configure.py` does not compile `src/`, does not set objdiff `base_path`, and does not run `lld-link` or `dtk shasum -c`. The ninja default is `build/report.json`. That is not the build in `notes/build.md`.
- `config/splits.txt` partitions `.text`. A unit is one original `.cpp` where an assert path in `Sum.exe` places that range, and a `bank/00xxxxxx` object where the `.cpp` is not known yet. Banks are cut on 4-aligned function starts, about 16KB each, so objdiff has one job per object. Do not collapse this back to a single `Sum.exe` unit. Do not emit one unit per `fn_` address. Do not link an unfinished `src/` tree with `/FORCE` to manufacture `build/Sum.exe`. A function stays unmatched until objdiff compares its object.
- `fn_004E6890` (size `0x66D0`) is a bad split: a jump table plus separate handlers. Real code in that span starts at `0x4E68E0`.
- `src/Engine/handler_4e68e0.cpp` matches retail under MSVC 6 `/O2` for `0x4E68E0` (0x44 bytes) and `0x4E6930` (0x6D bytes). The names in that file are not recovered. There is no `// FUNCTION: SUM` annotation, and the decomp.dev report does not include them.
