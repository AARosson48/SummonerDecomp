# Build notes (2026-10-01)

Read this before changing `configure.py`, `build.ninja`, the workflow, or the decomp.dev report. These are observations from source and one local link attempt. They are not a finished build.

## What the tools say the build is

`encounter/dtk-template` `tools/project.py` (saved locally as `build/dtk_project.py`, not committed):

- If `src/<unit>` exists, compile it. objdiff `base_path` is that compiled object (`generate_objdiff_config`, around the `src_path.exists()` check). Otherwise `base_path` stays empty.
- `all_source` is every compiled source object. `objdiff report generate` depends on `all_source`.
- A unit marked matching is linked from the compiled object. Otherwise the link input is the split object (`add_unit`).
- The default ninja target runs `dtk shasum -c` against `config/<version>/build.sha1`.
- That file's compiler rule is `mwcceppc.exe` and its linker is `mwldeppc.exe`, then `dtk elf2dol`. Those two programs are the GameCube path. Do not put them in this ninja file.

`openblack/decomp-toolkit` tag `v0.0.27` (the `dtk` this repo already runs) writes the PE link recipe in `src/util/rsp.rs`:

```
Usage: `lld-link @args.rsp @objs.rsp /OUT:foo.exe`
```

`/SECTIONVSIZE:` is emitted there from each section's `VirtualSize`. `src/util/coff.rs` adjusts `IMAGE_REL_I386_REL32` addends for lld-link, not for MSVC `link.exe`. `src/cmd/coff.rs` `split_write_coff` writes `args.rsp` and `objs.rsp`. Our command `dtk coff split` already did that. `build/base/args.rsp` and `build/base/objs.rsp` are that output (150 objects).

The PE adaptation of the template (`openblack/bw1-decomp` `docs/getting_started.md` and `docs/dependencies.md`) says the first `ninja` downloads `dtk`, `lld-link`, and the MSVC 6 compiler, splits, links the split objects, and checks SHA-1. A passing SHA-1 means the relinked image matches the original. Decompiling one file is: `Object(NonMatching)`, write C until objdiff matches the split object, then `Object(Matching)` so the link uses the compiled object. Their progress pages are not a source for our scores.

`lld-link` with `/SECTIONVSIZE` is the `llvm-windows.zip` asset on `openblack/llvm-project` release `bw1-decomp-024`:

https://github.com/openblack/llvm-project/releases/download/bw1-decomp-024/llvm-windows.zip

sha256 `20c197ccabf82c5a24dd2c9ecca12f47bb0cdcdbbbc3e3169e0be56a3f15252a`, size 198972730. Downloaded to `build/llvm/llvm-windows.zip`. Hash matched. `bin/lld-link.exe` extracted to `build/llvm/bin/lld-link.exe`. The zip stays out of git (`build/` is ignored).

## What this repo's ninja does

`configure.py` `write_objdiff` sets every `base_path` to `None`. `write_ninja` has two rules, `split` and `report`, and `default build/report.json`. It does not compile `src/`, does not call `lld-link`, and does not run `dtk shasum -c config/sum-pc/build.sha1`. CI is Ubuntu and only runs that ninja file. The report on decomp.dev is a map of the retail exe.

## What was run

MSVC 6 `link.exe` 6.00.8447, from `C:\projects\MSVC600\VC98\Bin\link.exe`:

```
link.exe @build/base/args.rsp @build/base/objs.rsp /OUT:build/sum-pc/Sum.exe
```

It ignored `/SECTIONVSIZE`, `/SAFESEH:NO`, `/errorlimit:0`, `/demangle:no`, `/includeglob:*`, then:

```
00454C44.o : fatal error LNK1143: invalid or corrupt file: no symbol for comdat section 0x1
```

Exit 1143. `build/sum-pc/Sum.exe` was not produced. That linker is the wrong program for `args.rsp`.

## lld-link run (2026-10-01)

Command, from the repo root, using `build/llvm/bin/lld-link.exe`:

```
lld-link @build/base/args.rsp @build/base/objs.rsp /OUT:build/sum-pc/Sum.exe
```

Exit 0. Output size 1,785,856, same as retail. `dtk shasum -c config/sum-pc/build.sha1` failed:

```
build/sum-pc/Sum.exe: FAILED
```

SHA-1 of `build/sum-pc/Sum.exe` is `4e6d7e2533ebffd3b2dc7cb5f8e2082672dd711b`. Retail is `cdc5d71d447a89e9a1c5e176c65c9b129321d7fc`.

318 bytes differ, all in `0x2` through `0x29f`. From `0x2a0` to the end of the file the bytes are identical. `.text`, `.rdata`, and the initialized `.data` raw ranges are identical, including virtual size, raw pointer, raw size, and characteristics.

The headers are not:

| field | retail | linked |
| --- | --- | --- |
| `e_lfanew` | `0x108` (Rich header in the DOS stub) | `0x80` (PE signature, no Rich header) |
| timestamp | `0x3BFBCD1C` (2001-11-21) | `0x6ABE0CC9` (link time) |
| file characteristics | `0x10F` | `0x103` |
| section count | 4 | 5 |

The fifth section comes from `build/base/obj/auto__02__005B3000__data.o`, which is a second `.data` with `IMAGE_SCN_CNT_UNINITIALIZED_DATA` and raw size `0x2A0C2F8`. lld-link kept it as its own `.data` at VA `0x2BC0000` (raw pointer 0) and moved `.rsrc` from VA `0x2BC0000` to `0x55F8000`. The `.rsrc` file bytes are still at raw `0x1B3000`. `src/util/rsp.rs` does not emit `/TIMESTAMP` or a Rich-header flag.

Do not change `splits.txt` from this log. Do not score the report in Python. The sha1 gate is still red, so `configure.py` stays on the report target until a link hash matches.

## Work order

The sha1 miss does not block function matching. `.text` / `.rdata` / initialized `.data` already match, so objdiff's target objects are the original code. Do these in order. Do not skip to a later step because an earlier one looks slow.

1. Compile `src/bank/00401000.cpp` with `C:\projects\MSVC600\VC98\Bin\cl.exe /nologo /O2 /c`. That file is the only `src/` unit in this list. The other 40 `.cpp` files are not compile edges until each one compiles on its own. `s3d.cpp` fails on MSVC 6 (`std::`, `for (int i` scope). Do not edit those files to force them in. Do not compile `src/Engine/handler_4e68e0.cpp` (invented names, not a splits.txt unit).
2. Point objdiff `base_path` for unit `bank/00401000` at that compiled object. `target_path` stays `build/base/obj/bank/00401000.o`. The report target depends on the compile (`all_source`). Every other unit keeps `base_path` empty.
3. Run objdiff on that pair and read its output before changing the C. Done once on 2026-10-01. `objdiff-cli diff -1 build/base/obj/bank/00401000.o -2 build/src/bank/00401000.obj` did not pair any function. Split symbols are `fn_00401000`. The `.cpp` compiled as C++ and emitted `?FUN_00401000@@YAXXZ`. The first split `.text` section (`fn_00401000`, 0x20 bytes) has **zero** relocations; the bytes are `33 C0 A3 E0 27 5B 00 ...` (`mov [0x5B27E0], eax`). objdiff prints that as an absolute address. The compiled object has `IMAGE_REL_I386_DIR32` to `?DAT_005B27E0@@3HA`. `0x5B27E0` is not in `config/dtk_symbols.txt`. `fn_00401070` (section size 0x120) has only two relocs: `fn_00401190` and `_sprintf` (type 20, REL32). The other calls and data stores in that function are still absolute. Adding `DAT_005B27E0`–`DAT_005B27EC` as `.data` objects made `dtk coff split` emit `IMAGE_REL_I386_DIR32` for those stores. objdiff still did not pair `fn_00401000` with MSVC's `_fn_00401000`. The symbol lines for these seven functions and four objects in `config/dtk_symbols.txt` now use the leading underscore MSVC emits. After that re-split, `objdiff-cli diff` on 2026-10-01 reported:

| symbol | match_percent |
| --- | --- |
| `_fn_00401000` | 100 |
| `_fn_00401020` | 100 |
| `_fn_00401040` | 100 |
| `_fn_00401050` | 100 |
| `_fn_00401060` | 100 |
| `_fn_00401190` | 100 |
| `_fn_00401070` | 99.11842 (27 `DIFF_ARG_MISMATCH`) |

`configure.py` compiles `src/bank/00401000.cpp` and sets that unit's `base_path` only when `C:\projects\MSVC600\VC98\Bin\cl.exe` exists. The unit stays `complete: false` and the split object stays in `objs.rsp`. On Windows the report rule is `cmd /s /c` because ninja does not run `&&` itself, and `tools/bin/...` is a switch to `cmd` (the slash). Ubuntu keeps `&&`.

`ninja build/report.json` on 2026-10-01 wrote `build/report.json`. Unit `bank/00401000` measures: `matched_code` 362, `matched_code_percent` 2.4048362, `matched_functions` 7 of 27. Project: `matched_code` 362 of `total_code` 1519213 (`matched_code_percent` 0.023828126), `matched_functions` 7 of 6826. Not uploaded.

`objdiff-cli diff` and the report agree at 100 for `_fn_00401000`, `_fn_00401020`, `_fn_00401040`, `_fn_00401050`, `_fn_00401060`, and `_fn_00401190`. They disagree on `_fn_00401070`: diff `match_percent` 99.11842 with 27 `DIFF_ARG_MISMATCH`, report `fuzzy_match_percent` 100.0, and those 279 bytes are inside the unit's `matched_code` 362. Examples of the diff, target then compiled:

```
mov eax, [eax*0x4+0x588a78]  ||  mov eax, [eax*0x4+_DAT_00588A70+0x8]
call 0x12830                 ||  call _FUN_004138A0
cmp byte ptr [0x2491a5c], 0x1 || cmp byte ptr [_DAT_02491A5C], 0x1
```

The split left those as absolute addresses. Same-unit `call _fn_00401190` is a reloc and matched. The split log says `dropped 26120 cross-unit relocation(s) to non-exportable targets`, which covers calls into other banks. Do not upload this report while it records `_fn_00401070` as fuzzy 100. Next: find the symbol attribute that keeps a cross-unit reloc, name the data addresses the diff still prints as immediates (`0x588A70`, `0x2491A5C`, `0x5A5740`, and the rest of that function), re-split, and stop when diff also says 100.
4. A function stays unmatched until that objdiff output says so. `bank/00401000` is `.text` `0x00401000`–`0x00404BD0` (~15KB). The seven functions are only the start. Leave the split object in `objs.rsp`. Do not mark the unit `complete` or swap it into the link until objdiff matches the whole object.
5. After the seven show up in an objdiff report, the next code in that bank is `fn_004011B0`, then `fn_004011D0`.
6. The exe SHA-1 stays a separate last gate: Rich header, timestamp `0x3BFBCD1C`, file characteristics `0x10F`, and the extra uninitialized `.data` from `auto__02__005B3000__data.o`. Do not change `splits.txt` to chase it. `configure.py`'s default target stays `build/report.json` until a link hash matches.
7. The report decomp.dev shows is the GitHub artifact `sum-pc_report`. A job that does not compile leaves every item at 0. The workflow runs on `windows-latest`, checks out `itsmattkc/MSVC600` to `msvc6/`, and sets `MSVC6_ROOT` so `configure.py` finds `cl.exe`. Do not invent match percents in `tools/scrub_report.py` to stand in for that.

## Still true, separate from the link

- Retail `Sum.exe` SHA-1 `cdc5d71d447a89e9a1c5e176c65c9b129321d7fc`. Private repo `AARosson48/SummonerDecomp-retail`. Public CI checks it out to `orig/sum-pc/`. Do not commit the exe.
- Rich header: Utc12_CPP build 8966. Do not copy another project's MSVC service-pack pin until this exe's link matches.
- `src/bank/00401000.cpp` has seven functions that matched retail bytes under `cl /O2` with relocs masked, in a one-off compile. They are not in the report, because `base_path` is null.
- Do not edit `src/` to force the whole tree to compile. `s3d.cpp` fails on MSVC 6 (`std::`, and `for (int i` scope). That is a compiler error for a file that is not in a matching link yet.
- Do not commit `src/Engine/handler_4e68e0.cpp`. The names in it were invented.
- Do not set match percents in `tools/progress.py` or `tools/scrub_report.py`. objdiff's `matched_data_percent: 100` on a unit with `total_data` 0 is not a match; the scrub removes those keys.
- `fn_004E6890` size `0x66D0` is a bad split (jump table). Code in that span starts at `0x4E68E0`.
