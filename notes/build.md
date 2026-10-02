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

1. Compile the units listed in `configure.py` `compiled_units()` with `C:\projects\MSVC600\VC98\Bin\cl.exe /nologo /O2 /c /I include`. That list includes the first bank, the table search paths, the file openers, the typed table readers in `vfs_file.cpp`, the text helpers in `text.cpp`, the allocator, and the packfile reader. Do not compile `src/Engine/handler_4e68e0.cpp`.
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

`configure.py` compiles `src/bank/00401000.cpp` and sets that unit's `base_path` only when `cl.exe` exists. The compile rule calls `cl` by full path, sets `INCLUDE`, and sets `PATH` to `VC98\Bin` and `Common\MSDev98\Bin` only. `MSPDB60.DLL` lives in the second directory; without it `cl` exits `0xC0000135`. Do not append the machine `%PATH%`. `if not exist ... && cl` returns 0 without compiling once `build\src\bank` exists; `mkdir ... & cl` always compiles. `/Fo` is quoted so the forward slashes in `$out` are not extra `cl` switches. The unit stays `complete: false` and the split object stays in `objs.rsp`. On Windows the report rule is `cmd /s /c` because ninja does not run `&&` itself, and `tools/bin/...` is a switch to `cmd` (the slash). Ubuntu keeps `&&`.

`ninja build/report.json` on 2026-10-01 wrote `build/report.json`. Unit `bank/00401000` measures: `matched_code` 362, `matched_code_percent` 2.4048362, `matched_functions` 7 of 27. Project: `matched_code` 362 of `total_code` 1519213 (`matched_code_percent` 0.023828126), `matched_functions` 7 of 6826. Not uploaded.

`objdiff-cli diff` and the report agree at 100 for `_fn_00401000`, `_fn_00401020`, `_fn_00401040`, `_fn_00401050`, `_fn_00401060`, and `_fn_00401190`. They disagree on `_fn_00401070`: diff `match_percent` 99.11842 with 27 `DIFF_ARG_MISMATCH`, report `fuzzy_match_percent` 100.0, and those 279 bytes are inside the unit's `matched_code` 362. Examples of the diff, target then compiled:

```
mov eax, [eax*0x4+0x588a78]  ||  mov eax, [eax*0x4+_DAT_00588A70+0x8]
call 0x12830                 ||  call _FUN_004138A0
cmp byte ptr [0x2491a5c], 0x1 || cmp byte ptr [_DAT_02491A5C], 0x1
```

The split left those as absolute addresses. Same-unit `call _fn_00401190` is a reloc and matched. The split log says `dropped 26120 cross-unit relocation(s) to non-exportable targets`, which covers calls into other banks. Do not upload this report while it records `_fn_00401070` as fuzzy 100. Next: find the symbol attribute that keeps a cross-unit reloc, name the data addresses the diff still prints as immediates (`0x588A70`, `0x2491A5C`, `0x5A5740`, and the rest of that function), re-split, and stop when diff also says 100.
4. A function stays unmatched until that objdiff output says so. `bank/00401000` is `.text` `0x00401000`–`0x00404BD0` (~15KB). The seven functions are only the start. Leave the split object in `objs.rsp`. Do not mark the unit `complete` or swap it into the link until objdiff matches the whole object.
5. The recovered readers are on the compare. Descent 4, Red Faction, and Summoner are the same Volition player in that order. Red Faction's assert paths are `D:\projects\rf_pc\vsdk\...` and a February 2001 Red Faction source tree has the same files Summoner still names: `vsdk/os/cmdline.cpp`, `vsdk/os/memory.cpp`, `vsdk/parse/parse.cpp`, `vsdk/math/matrix.cpp`, `vsdk/gr/gr.cpp`, `vsdk/bmpman/targautils.cpp`, `vsdk/vfile/file_packfile.cpp`, `vsdk/ca/character.cpp`, `vsdk/ca/character_instance.cpp`, `vsdk/ca/skeleton.cpp`, `gamesrc/player/player.cpp`, and `gamesrc/gamewide/gamesound.cpp`. The SDK and the engine are that shared code. A `.vpp` is just what the player opens. `packfile_init` opens `tables.vpp`. `vfs_file.cpp` reads fields. `text.cpp` is the string helper. `vsdk/parse/parse.cpp` is the scanner those scripts call. objdiff: require-key 99.52, quoted read 76.01, skip-block 88.70, skip-space 75.96, key match 61.21.

`python tools/tbl_map.py` reads every file in `VPP/tables` and writes `notes/tbl_map.md` and `notes/tbl_map.json` on this machine. Those two files are extracted table data. They stay out of git. Read the local map before writing C for a function. The headers listed on a function are the record it looks up, and the tables are the files that contain that record. The map does not assign a match percent.

objdiff's three code numbers, from `objdiff_core::bindings::report::Measures`:

- `fuzzy_match_percent` is the average similarity, including functions that only partly match. decomp.dev draws that as the fuzzy band. A function under 100 is blue.
- `matched_code_percent` is bytes in functions that match completely. That is the perfect-match / decompiled number. A function at 100 is green.
- `complete_code_percent` is code in units marked `metadata.complete` (linked in place of the split object). That is fully linked.

`levelscripts/script_internal.cpp` is `fn_004DB420` (`_fn_004DB420`, 716 bytes). It reads one navpoint: `$Name`, then `$Type` (`level`, `cutscene`, `camera`, `ambient sound`) or a `$npc` / `$player` / `$c_npc` / `$camera` name, then `+Plane`, `$Position`, and `$Orientation`. `level` and `cutscene` are dropped onto the ground. `objdiff-cli diff` reports 80.85859. The report unit is `fuzzy_match_percent` 81.09596. That is under 100, so it stays unmatched. The Level scripts category is the blue band for that function.

`bank/004D39C0` holds `_fn_004D39C0` (1632 bytes). The table map reads it as the quest list: `events.tbl` and `demo_events.tbl`, with `$Quest`, `+Main quest`, `+Stage`, and a `$Level` / `+Flag` block. `objdiff-cli diff` reports 68.902435. The report function is `fuzzy_match_percent` 69.47228. That is under 100, so it stays unmatched. The rest of that bank has no C, so the unit fuzzy is 7.1675286.

`vsdk/os/cmdline.cpp` is one function, `_fn_0053E310` (176 bytes). Red Faction's source tree has the same file. It walks the argument list, accepts a registered option that may take the next argument, and fails on an unrecognized `-` parameter. `objdiff-cli diff` reports 68.26316. The report function is 68.49123. Under 100, so it stays unmatched.

The same SDK files, each one function, now on the compare. `objdiff-cli diff` / report:

- `vsdk/math/matrix.cpp` turns a rotation matrix into an axis and an angle. `?fn_00510390@Mat3@@QAEXPAUVec3@@PAM@Z`: diff 93.723076, report 93.823074.
- `vsdk/os/registry.cpp` `_fn_00511F90`: diff 99.8. The report records 100 and counts its 179 bytes in `matched_code`. Same disagreement as `_fn_00401070`. The diff is not 1:1, so it stays unmatched.
- `vsdk/os/stringpool.cpp` `?fn_00544C40@StringPool@@QAEPADPBD@Z`: diff 61.431034, report 61.551723.
- `vsdk/gr/opengl/gr_opengl.cpp` `_fn_0054F6F0`: diff 94.69388, report 95.081635. Window mode `0xC8` / `0xC9` / `0xCA`, then `GL_VERSION` / `GL_EXTENSIONS` for `1.2` or `GL_APPLE_packed_pixel`.

The same Red Faction files, next pass. `objdiff-cli diff` / report:

- `Summoner/player/player.cpp` `_fn_004334F0` takes a slot off the free list. The assert string is `No more player slots availeble`. Diff 99.675. The report records 100 and counts its 141 bytes. The diff is not 1:1.
- `vsdk/gr/gr.cpp` `_fn_00506110` reads `SST_DUALHEAD` when the window mode is `0xC9`. Diff 91.95238, report 92.38095. The three accessors `_fn_00506160`, `_fn_00506170`, `_fn_00506180` are diff 99.666664, 99.5, 99.5. The report records those three as 100. The diff is not 1:1.
- `vsdk/ca/character.cpp` `?fn_00541240@Character@@QAEXHH@Z` is an objdiff 1:1 match, 36 bytes. **[STATE 3].** `_fn_00541290` diff 95.51724, `_fn_00541330` diff 95.5, `_fn_00541300` diff 90.0, `?fn_00541270@Character@@QAEHXZ` diff 49.090908.
- `vsdk/ca/character_instance.cpp` `?fn_0051ABD0@CharacterInstance@@QAEPAUVec3@@PAU2@@Z` sums the active animation samples. Diff 78.425, report 78.5375.

`vsdk/ca/skeleton.cpp` is the Red Faction skeleton file. `objdiff-cli diff` / report:

- `?fn_00543D50@Anim@@QAEPAU1@PBD0DHD@Z` copies the skeleton name, interns the second string, and fails with `Too many skeletons` at line `0x96`. Diff 77.327866, report 77.508194.
- `?fn_00543E10@Anim@@QAEXXZ` unlinks the skeleton and returns its allocations to the heap. Diff 72.906975, report 73.00775.
- `?fn_00543FC0@Anim@@QAEPAUVec3@@PAU2@@Z` is the bone sample `character_instance` calls. Scale is `1 / ((end - start) * 0.00625 * 0.033333335)`. Diff 77.0, report 77.08.

All three are under 100, so they stay unmatched. `fn_00544090` in that file has no C yet.

`Engine/gamesound/gamesound.cpp` is `_fn_004691E0`. It reads `music.tbl` (`$Name` / `$Filename`), then `feedback.tbl` for `Rosalind`, `Flece`, `Joseph`, and `Jekhar`. `objdiff-cli diff` reports 93.223175. The report function is 93.45493. Under 100.

The large functions in `Engine/Objects/living_entity.cpp` and the 8,864-byte function at `0x450E30` are compiled with a frame pointer. `_fn_0045CCB0` (77 bytes) is an objdiff 1:1 match under `#pragma optimize("", off)`. **[STATE 3].** `?fn_0045D4C0@Entity@@QAEXXZ` stores the `BDBN-Foot-L` / `BDBN-Foot-R` bones. Diff 73.45238, report 73.5.

`Engine/characterinfo/characterinfo.cpp` `_fn_00447E80` is the name hash (`rol` by 6, then xor). Diff 96.92308, report 96.92308. `_fn_00446DE0` walks four info rows. Diff 99.888885. The report records 100 and counts its 51 bytes. The diff is not 1:1, so it stays unmatched. The 5,200-byte reader in that file has no C yet.

`vsdk/os/text.cpp` compare helpers are objdiff 1:1. **[STATE 3].** `_vfs_text_eq` 59, `_vfs_text_ne` 60, `_vfs_text_lt` 67, `_vfs_text_gt` 70, `_vfs_text_le` 70, `_vfs_text_ge` 67. They return `bool` and call `_stricmp`. `@vfs_text_clear@4` 28, `@vfs_text_empty@4` 22, and `@vfs_text_blank@4` 51 are `__fastcall` and also 1:1. The `*cstr` overloads stay under 100.

The report after those text matches: `fuzzy_match_percent` 1.0225604, `matched_code` 1443, `matched_code_percent` 0.09502273, `matched_functions` 29 of 6916. The new matched bytes are the nine text helpers (494).

`_vfs_init_search_paths` (636 bytes) is an objdiff 1:1 match. **[STATE 3].** It registers the table search paths, including the `data\tables` id at `0x60AC5C`. The path strings and those ids are named in `config/dtk_symbols.txt` so the relocations pair.

The report after that function: `fuzzy_match_percent` 1.0228825, `matched_code` 2079, `matched_code_percent` 0.13690385, `matched_functions` 30 of 6916.

`?fn_0045DEA0@Entity@@QAEXHHHHHHHH@Z` is the 8,661-byte hit handler in `Engine/Objects/living_entity.cpp`. It is compiled with the frame pointer on. The recovered front walks the player list, fans `0x48` out into kinds `7`, `8`, and `0x43`, then picks a damage amount from the damage-type switch (`Item special`, `Reflex abil`). The action switch now has kinds `0`/`4`, `1`, `3`/`5`/`0x69`, `0xC`/`0x54`/`0x57`/`0x65`, `0xD`, `0xF`, `0x66`, `0x68`, `0x6A`, and `0x6B`, plus the shared tail. `objdiff-cli diff` reports 36.378 on the full 8,661 bytes. Kinds such as `0x17` and the `get_hit_by_spell` path are still missing. Under 100.

The report after that handler: `fuzzy_match_percent` 1.2311891, `matched_code` 2079, `matched_code_percent` 0.13690385, `matched_functions` 30 of 6916. Matched bytes did not change. `complete` stays false on every unit, so there is still no fully-linked band.
6. The exe SHA-1 stays a separate last gate: Rich header, timestamp `0x3BFBCD1C`, file characteristics `0x10F`, and the extra uninitialized `.data` from `auto__02__005B3000__data.o`. Do not change `splits.txt` to chase it. `configure.py`'s default target stays `build/report.json` until a link hash matches.
7. `bank/` units were left out of `progress_categories`, so decomp.dev's groups stayed at 0 while the project total included their matches. `bank/*` is the Banks category. Engine, Volition SDK, Game, and Level scripts only count units whose split name is an original `.cpp`. The report decomp.dev shows is the GitHub artifact `sum-pc_report`. A job that does not compile leaves every item at 0. The workflow runs on `windows-latest`, checks out `itsmattkc/MSVC600` to `msvc6/`, and sets `MSVC6_ROOT` so `configure.py` finds `cl.exe`. Do not invent match percents in `tools/scrub_report.py` to stand in for that.

## Still true, separate from the link

- Retail `Sum.exe` SHA-1 `cdc5d71d447a89e9a1c5e176c65c9b129321d7fc`. Private repo `AARosson48/SummonerDecomp-retail`. Public CI checks it out to `orig/sum-pc/`. Do not commit the exe.
- Rich header: Utc12_CPP build 8966. Do not copy another project's MSVC service-pack pin until this exe's link matches.
- `src/bank/00401000.cpp` is on the report. The original six functions in that file still match. `_fn_00401070` is still the diff-versus-report disagreement in step 3. That function is the interface mode runner. Fifteen modes are triples of callbacks at `0x588A70`. Mode 2 plays `Title`, beside `credits.tbl` and `geeks.bik`. Mode 8 builds a screen with a `Load` button and a `Cancel` button. The panels in this bank are `Assess-Bkgrnd` (`fn_004011D0` and the functions from `0x401380`), `Map-Bkgrnd` (`fn_00401EF0`), and `BuySell-Bkgrnd` (`fn_00404A20`). `Options-Bkgrnd` is referenced from `0x41EF46` and `SaveLoad-Bkgrnd` from `0x425510`.
- `gap_00_00402250_text` is cut into functions from `0x402250` through `0x4044A0`. It sits between the map panel and the automap commands. The int at `0x60AD68` selects the scale: 1.0, 1.25, or 1.6. The int at `0x5B2A10` is a step, 0 through 6, dispatched by `_fn_00402680`. `objdiff-cli diff` is 100 for `_fn_00402250` (44 bytes), `_fn_004025A0` (65), `_fn_00402FB0` (27), `_fn_00404460` (6), and `_fn_00404470` (45). **[STATE 3].** The other new functions stay under 100. `_fn_00402560` diff 98.92857, report 99.28571. `_fn_00402F10` diff 82.85, report 82.94444. `_fn_00402FD0` diff and report 81.36364. `_fn_00402490` diff 79.55, report 79.66129. `_fn_00402680` diff 73.64, report 74.175674. `_fn_004023B0` diff 63.26, report 63.353848. `_fn_004025F0` diff and report 59.61111.
- The report after that split: `fuzzy_match_percent` 1.2935994, `matched_code` 2266, `matched_code_percent` 0.14923948, `matched_functions` 35 of 6945, `total_code` 1518365. The new matched bytes are those five functions (187). `complete` stays false.
- The assess span from `0x401380` is cut into functions. `_fn_00401380` opens the panel. `_fn_004017F0` dispatches its step. `_fn_00401A20`, `_fn_00401A50`, `_fn_00401AD0`, and `_fn_00401AF0` move the cursor at `0x5B28B8`. `_fn_00401A80` is the button handler (sounds `0x25` and `0x24`). The automap commands are `map_mask`, `map_grid`, `show_coordinates`, and `reset_map_labels` (`_fn_004044D0`, `_fn_004045C0`, `_fn_004046B0`, `_fn_004047A0`). `objdiff-cli diff` is 100 for `_fn_004017E0` (15 bytes), `_fn_00401AD0` (21), and `_fn_00401E50` (45). **[STATE 3].** `_fn_00401380` diff and report 99.6. The command handlers are under 100 (diff 77.04, and `reset_map_labels` 90.09).
- The report after that: `fuzzy_match_percent` 1.3678763, `matched_code` 2347, `matched_code_percent` 0.15458994, `matched_functions` 38 of 6972, `total_code` 1518210. The new matched bytes are those three functions (81). `complete` stays false.
- Assess now has C for the image load (`_fn_004011D0`), the description builder (`_fn_004013F0`), the scrollbar (`_fn_004016D0`), the click handler (`_fn_00401840`), and the draw (`_fn_00401C80`, `_fn_00401CE0`, `_fn_00401DA0`). The seven images at `0x57E4C0` are `Assess-Bkgrnd` and the arrow and close icons, each plus `640.tga` or `1024.tga` from `0x58BDA4`. **[STATE 2].** Report fuzzy: `_fn_00401C80` 91.55556, `_fn_00401CE0` 87.36207, `_fn_00401B20` 86.5, `_fn_004013F0` 77.427986, `_fn_004011D0` 71.97059, `_fn_00401DA0` 61.568626, `_fn_004016D0` 56.923077, `_fn_00401BF0` 53.63158, `_fn_00401840` 20.93421. None of these is 100.
- The report after those bodies: `fuzzy_match_percent` 1.4795859, `matched_code` 2347, `matched_code_percent` 0.15458994, `matched_functions` 38 of 6972, `total_code` 1518210. Matched bytes did not change. `complete` stays false.
- `_fn_00401EF0` loads the map. Nine images at `0x57E6B8`: `Map-Bkgrnd`, the left and right arrows, `MapRecenter`, and the close icon, then `hud-mapmask01.tga`, `Map-Label`, `hud-mapcamera.tga`, and `You-R-Here.tga`. `_fn_00404A20` loads Buy/Sell, forty images at `0x588E08` from `BuySell-Bkgrnd` through the close icon, plus `ScrollBarMid`. `_fn_00404A00` loads `bs_cameras.tbl`. The old `fn_004048B0` span is four functions: the constructor thunk, the selection flags, and two row lookups. `objdiff-cli diff` and the report are 100 for `_fn_00404A00` (19 bytes). **[STATE 3].** The others stay under 100. Report fuzzy: `_fn_00401EF0` 82.783676, `_fn_00404A20` 76.37313, `_fn_004049C0` 67.63158, `_fn_004048D0` 53.45, `_fn_00404960` 48.974358. **[STATE 2].**
- The report after the map and Buy/Sell: `fuzzy_match_percent` 1.5586185, `matched_code` 2366, `matched_code_percent` 0.15584397, `matched_functions` 39 of 6975, `total_code` 1518185. The new matched bytes are `_fn_00404A00` (19). `complete` stays false.
- `_fn_0042FFE0` is the callee at `0x42FFE0`. `fn_0042FF50` ends there, and the function is 0x50 bytes. It writes two floats through the first two arguments and loads the next two stack slots with `fld`. `_fn_004023B0` and `_fn_00402490` pass the dwords at `+0x10` and `+0x18` and multiply those floats by the scale. The scale switch has an explicit case 0, which is the `sub eax, 0` chain. **[STATE 2].** Report fuzzy: `_fn_00402490` 85.91936, `_fn_004023B0` 66.292305. `objdiff-cli diff` prints 85.92 and 66.29. `fn_00413460` returns `al`. The two functions that return its result, `_fn_00401E50` and `_fn_00404470`, are still 100, so the prototype stays `int`.
- The report after that callee: `fuzzy_match_percent` 1.5598936, `matched_code` 2366, `matched_code_percent` 0.15584397, `matched_functions` 39 of 6975, `total_code` 1518185. Matched bytes did not change. `complete` stays false.
- The callees of the other large under-100 functions were read in the whole executable. `fn_004A28B0` returns `al`. The hit handler is compiled with the frame pointer on, so the caller masks with `and eax, 0xff` before the test. The prototype is now `unsigned char`. `objdiff-cli diff` reports 36.41 on the 8,661 bytes. The report scores that function `fuzzy_match_percent` 36.55799. **[STATE 2].** The quest reader, the navpoint reader, the map loader, and the assess description have no byte-return or float-argument mismatch of that kind.
- The report after that return: `fuzzy_match_percent` 1.5600895, `matched_code` 2366, `matched_code_percent` 0.15584397, `matched_functions` 39 of 6975, `total_code` 1518185. Matched bytes did not change. `complete` stays false.
- `_fn_00401A50` pages the assess list forward. Loading the cursor before the step makes the registers match, and the call to `_fn_004016D0` is a tail `jmp`. `objdiff-cli diff` and the report are 100 for those 48 bytes. **[STATE 3].** `_fn_00401A80` switches on the button id with `sub eax, 2` for ids 2, 4, and 6. `objdiff-cli diff` is 99.81481. The one remaining row is `call 0x416A10` against `call _fn_00416A10`, because that address has no symbol and the split left the call as an immediate. The report scores `_fn_00401A80` at 100.0, so its 80 bytes are inside `matched_code`. **[STATE 2].**
- The report after those two: `fuzzy_match_percent` 1.560332, `matched_code` 2494, `matched_code_percent` 0.16427511, `matched_functions` 41 of 6975, `total_code` 1518185. The new bytes in `matched_code` are `_fn_00401A50` (48) and the report's 100 for `_fn_00401A80` (80). `complete` stays false.
- Functions of 32 bytes or less were read across every bank. The ones that are a load, a store, or a constant return are in the new `src/bank/` files, compiled `/O2`. The one-instruction methods that read or write through `ecx` are `__fastcall` (`@fn_...@4`), including four appended to `vfs_file.cpp` and `file_packfile.cpp`. `objdiff-cli diff` is 100 on all 106 of them (478 bytes of loads and stores, 225 bytes of methods). **[STATE 3].** Compiler thunks, import jumps, and exception helpers in that size range are still not C.
- The report after those functions: `fuzzy_match_percent` 1.6070812, `matched_code` 3197, `matched_code_percent` 0.2105804, `matched_functions` 147 of 6975, `total_code` 1518185. The new matched bytes are those 106 functions (703). `complete` stays false.
- Callers of those functions were disassembled from each function start. The next functions are the same shape, and three addresses had no symbol even though the previous function ended there: `0x4158B0`, `0x4872B0`, and `0x501860`. `objdiff-cli diff` is 100 for 13 of them (223 bytes). **[STATE 3].** `?fn_00473610@Obj73610@@QAEXH@Z` (10) stores through `ecx` and pops one argument. `@fn_00493F70@4` (4), `@fn_004943F0@4` (13), `@fn_004DAC20@4` (11), `@fn_00542FF0@4` (11), and `@fn_00501860@4` (9) are `__fastcall`. `_fn_0051F540` (10), `_fn_00481E50` (23), `_fn_00530C20` (23), `_fn_00568B20` (16), `_fn_00477780` (25), `_fn_004872B0` (17), and `_fn_004158B0` (51) are cdecl. `0x530920` is `dec dword ptr [0x2cbfcc8]`. `/O2` loads that counter into `eax`, so that function stays out.
- The report after those callers: `fuzzy_match_percent` 1.6217698, `matched_code` 3420, `matched_code_percent` 0.22526897, `matched_functions` 160 of 6975, `total_code` 1518185. The new matched bytes are those 13 functions (223). `complete` stays false.
- The callers of those helpers include three deleting destructors. Each keeps the object in `ecx`, calls the one-instruction setup, frees the object when the flag bit is set, and returns it with `ret 4`. `objdiff-cli diff` is 100 for `?fn_00493F40@Obj493F40@@QAEPAXI@Z` (30 bytes), `?fn_0052F400@Obj52F400@@QAEPAXI@Z` (30), and `?fn_00543010@Obj543010@@QAEPAXI@Z` (30). The store at `?fn_0052C690@Obj52C690@@QAEXHH@Z` pops 8 bytes and is 100 (18). `_fn_00431E00` sets the byte at `0x5FC01C`, calls `_fn_00432710` and `_fn_004119A0`, and tail-jumps to `_fn_00411CD0`. It is 100 (26). **[STATE 3].** `_fn_00546130` walks one slot and clears a byte. `objdiff-cli diff` and the report are 94.28571. Retail keeps the index in `ecx` and the slot in `eax`. **[STATE 2].**
- `_fn_00401070` names its callees `_fn_00414020`, `_fn_00501510`, `_fn_00500A90`, `_fn_005011E0`, `_fn_004413D0`, `_fn_004408A0`, `_fn_00431640`, and `_fn_004328B0`, and the globals those calls use are `_DAT_` symbols. `objdiff-cli diff` is 99.960526 on the 279 bytes. The report scores the function `fuzzy_match_percent` 100.0, so those 279 bytes stay inside `matched_code`. Three rows remain: the mode table at `0x588A70` is an immediate on the retail side and `_DAT_00588A70+n` on the compiled side. **[STATE 2].**
- The report after those callers: `fuzzy_match_percent` 1.6320602, `matched_code` 3554, `matched_code_percent` 0.2340953, `matched_functions` 165 of 6975, `total_code` 1518185. The new matched bytes are the five functions (134). `complete` stays false.
- `_vfs_path_id_ok` returns through `al`. The slot pointer is the immediate at `0x2693068`, which is `g_search[path_id].path`. `objdiff-cli diff` is 100 on those 46 bytes. **[STATE 3].** `_fn_00401380` reads `DAT_005FBFD8`, so that byte load pairs. `objdiff-cli diff` is 99.6. The two remaining rows load the argument with `eax` on the retail side and `ecx` on the compiled side. **[STATE 2].**
- The report after that check: `fuzzy_match_percent` 1.632143, `matched_code` 3600, `matched_code_percent` 0.23712525, `matched_functions` 166 of 6975, `total_code` 1518185. The new matched bytes are `_vfs_path_id_ok` (46). `complete` stays false.
- Short functions that had a symbol and no C, in banks that already compile, are now C. `objdiff-cli diff` is 100 on 36 of them (483 bytes). **[STATE 3].** They are stores, one-call wrappers, and methods that keep the object in `ecx`. The wrappers call `_fn_00412620`, `_fn_00410760`, `_fn_00401020`, `_fn_004F1EB0`, `_fn_004CE6D0`, and `_fn_00530CB0`. The bit tests in the packfile shift an unsigned value. `?fn_00544B20@Obj44B20@@QAEPAXH@Z` returns the object.
- The report after those functions: `fuzzy_match_percent` 1.6639575, `matched_code` 4083, `matched_code_percent` 0.26893955, `matched_functions` 202 of 6975, `total_code` 1518185. The new matched bytes are those 36 functions (483). `complete` stays false.
- The SDK near-misses that were a relocation or a signed compare are now 1:1. `objdiff-cli diff` is 100 for `_fn_00511F90` (179 bytes, the `HKEY_` names), `_fn_00506110` (73, `SST_DUALHEAD` through `GetEnvironmentVariableA`), `_fn_00506160` (10), `_fn_00506170` (6), `_fn_00506180` (6), `?fn_00541270@Character@@QAEHXZ` (31), `_fn_00541290` (107), and `_fn_00541330` (47). **[STATE 3].** The absolute addresses outside the image stay immediates. The strings and the character-table pointers are named so the relocations pair. `_fn_00541300` is 90.0. Retail zeros the two middle fields through `edx`, and this compiler reuses `ecx`. **[STATE 2].**
- The report after those SDK functions: `fuzzy_match_percent` 1.6657876, `matched_code` 4341, `matched_code_percent` 0.28593355, `matched_functions` 206 of 6975, `total_code` 1518185. The new matched bytes are `_fn_00506110`, `?fn_00541270@Character@@QAEHXZ`, `_fn_00541290`, and `_fn_00541330` (258). The registry check and the three accessors were already inside `matched_code` from the earlier report-versus-diff disagreement, and the diff is now 100 as well. `complete` stays false.
- The save header and inventory functions in `bank/004CFF80` now have C and build. `objdiff-cli diff` is 70.223076 for `?fn_004D1DF0@SaveBuf@@QAEHPAD@Z` (writes `SAVg`, version `0x27`, the two area names, the time at `0x60A654`, and the description), 48.0229 for `?fn_004D29F0@SaveBuf@@QAEHPAD0PAH101@Z`, 70.59322 for `?fn_004D2090@SaveBuf@@QAEHXZ` (`INVT`, gold at `0x971868`), and 79.21649 for `?fn_004D2C90@SaveBuf@@QAEHXZ`. **[STATE 2].** `summoner0.sav` uses that layout: area `masad`, gold 30, Joseph as the only type-7 character. `_fn_004D25A0` in the same bank stays 100.
- The report after those save functions: `fuzzy_match_percent` 1.7194443, `matched_code` 4341, `matched_code_percent` 0.28593355, `matched_functions` 206 of 6975, `total_code` 1518185. Matched bytes did not change. `complete` stays false.
- The five multiplayer screen builders now have C and build. `notes/multiplayer.md` is the layout. `objdiff-cli diff` is 25.171875 for `_fn_004063F0` (character select, 34 slots at `0x5B6940`), 13.563637 for `_fn_004099D0` (create game), 26.323076 for `_fn_0040A6A0` (game list), 25.787233 for `_fn_004191B0` (trade), and 16.610687 for `_fn_0041BEE0` (level select). **[STATE 2].** Each one loads the widget name plus the `640.tga` / `1024.tga` suffix, then the rectangle from the language slot. `_fn_004063A0`, `_fn_0040ACC0`, and `_fn_00419160` in those banks stay 100.
- The report after those builders: `fuzzy_match_percent` 1.7486894, `matched_code` 4341, `matched_code_percent` 0.28593355, `matched_functions` 206 of 6975, `total_code` 1518185. Matched bytes did not change. `complete` stays false.
- The other menu builders and the opening logos now have C and build. `objdiff-cli diff` is 19.073172 for `_fn_0041D740` (title, 13 slots at `0x5C2F98`; a `.` in the name skips the suffix and the scale), 82.608696 for `_fn_0041D8B0` (`vlogo.bik`, then `eaxlogo.bik` while `0x5930BD` is set, then `"Title"`), 16.79878 for `_fn_00408D80` (dialogue box, two slots at `0x5BC458`), 24.925676 for `_fn_00414DE0` (inventory), 26.547445 for `_fn_004185A0` (item popup), 26.097015 for `_fn_0041A960` (quest journal), 23.896551 for `_fn_0041EF30` (options, 75 slots, flag on 0, 1, 2, 19, and 70), 12.631147 for `_fn_00421590` (pause popup), 29.877697 for `_fn_004254E0` (save/load), and 29.775757 for `_fn_0042B260` (skills, in `bank/00429410`). **[STATE 2].** The functions that were already 100 in those banks stay 100.
- The report after those menus: `fuzzy_match_percent` 1.8144567, `matched_code` 4341, `matched_code_percent` 0.2859371, `matched_functions` 206 of 6978, `total_code` 1518166. Matched bytes did not change. `total_code` is 19 bytes smaller because `_fn_0041D8B0` is now its own symbol in the gap after `_fn_0041D740`. `complete` stays false.
- New Game's name copy and the mode 2 enter now have C and build. `notes/newgame.md` is the live path. `objdiff-cli diff` is 40.9 for `_fn_00431A40` (copies the level name to `0x60A2C8`, picks a variant into `0x60A2E8`, sets the intro byte, requests mode 2) and 86.521736 for `_fn_00431E20` (plays `"Title"`, sets gold to 30, then requests mode 4 from its tick `_fn_00431E70`). **[STATE 2].** The functions that were already 100 in `bank/0042F7D0` stay 100. `_fn_00431E70` was already C.
- The report after those two functions: `fuzzy_match_percent` 1.8267022, `matched_code` 4341, `matched_code_percent` 0.2859371, `matched_functions` 206 of 6978, `total_code` 1518166. Matched bytes did not change. `complete` stays false.
- The level load now has C and builds. `objdiff-cli diff` is 100 for `_fn_004DAEF0@8` (92 bytes) and `?fn_004D7C70@ObjD7C70@@QAEXXZ` (47). **[STATE 3].** `?fn_004D82F0@ObjD7C70@@QAEHPAD0@Z` is 56.51923 (147). **[STATE 2].** It calls `fn_004FEE50` and then either `fn_004D84D0` or `fn_004D02D0`. The functions that were already 100 in `bank/004D7C70` stay 100.
- The report after that load: `fuzzy_match_percent` 1.8413308, `matched_code` 4480, `matched_code_percent` 0.29509288, `matched_functions` 208 of 6978, `total_code` 1518166. The new matched bytes are `_fn_004DAEF0@8` and `?fn_004D7C70@ObjD7C70@@QAEXXZ` (139). `complete` stays false.
- The S3D header read now has C and builds. `objdiff-cli diff` is 100 for `_fn_0046F6D0` (412 bytes) under MSVC 6 `cl` 12.00.8804 `/O2`. **[STATE 3].** It reads the version dword after `S3DF`, stores it with `vfs_file_set_user`, then the nineteen counts and the three color floats. The same bank's name walks are under 100: `_fn_0046FFA0` 62.846153, `_fn_00470150` 68.44827, `_fn_004701E0` 60.73077, `_fn_00470560` 45.0. **[STATE 2].** `_fn_00470BB0` stays 100.
- The report after that header: `fuzzy_match_percent` 1.8712533, `matched_code` 4892, `matched_code_percent` 0.3222309, `matched_functions` 209 of 6978, `total_code` 1518166. The new matched bytes are `_fn_0046F6D0` (412). `complete` stays false.
- Player starts and the level-script init now have C in `levelscripts/Scripts/level_scripts_common.cpp`. `objdiff-cli diff` is 100 for `_fn_004CAF40` (202 bytes) under MSVC 6 `cl` 12.00.8804 `/O2`. **[STATE 3].** It looks up `$player%d-%02d` and, if the first marker is missing, `$player%02d`. Those are the marker names at the end of the `.s3d`. The assert path names this `.cpp` and says it cannot locate player starts. `_fn_004CB010` is 91.458336 (walks the party list at `0x5FC350`). `_fn_004CB3A0` is 85.793106 (zeros the level-script state, then calls the navpoint reader, the level-item reader, the `masad.tbl` reader, the player-start lookup, and `fn_004CEDB0`). **[STATE 2].**
- The report after that init: `fuzzy_match_percent` 1.8966227, `matched_code` 5094, `matched_code_percent` 0.33553642, `matched_functions` 210 of 6978, `total_code` 1518166. The new matched bytes are `_fn_004CAF40` (202). `complete` stays false.
- Bank `004CC010` now compiles. It holds the 10-slot table the level-script init clears, and the wrappers that resolve a name before calling the real body. `objdiff-cli diff` is 100 for `_fn_004CCC50` (21), `_fn_004CEDB0` (22), `_fn_004CEE90` (47), `_fn_004CE1C0` (32), `_fn_004CE320` (25), `_fn_004CE3A0` (32), `_fn_004CE540` (35), `_fn_004CE600` (30), `_fn_004CE620` (25), and `_fn_004CE6D0` (25) under MSVC 6 `cl` 12.00.8804 `/O2`. **[STATE 3].** `_fn_004CEDB0` writes `-1` through the ten slots at `0x2491A70`. `_fn_004CED90` is 44.0 and `_fn_004CEDD0` is 71.818184. **[STATE 2].**
- The report after that bank: `fuzzy_match_percent` 1.9181716, `matched_code` 5388, `matched_code_percent` 0.3549019, `matched_functions` 220 of 6978, `total_code` 1518166. The new matched bytes are those ten functions (294). `complete` stays false.
- The `masad.tbl` readers in that bank now have C and build. `objdiff-cli diff` is 95.026085 for `_fn_004CC010` (an `$Ambient` line: marker, wav, three numbers, and an optional fourth scaled by 1000), 45.31111 for `_fn_004CC190` (a `$Trigger` line: `+Id` is `load level`, `boss`, or `sound`; `+Type` is `spline` or `location`), and 92.5 for `_fn_004CCF00` (starts a cutscene, or calls the completion pointer when cutscenes are suppressed). **[STATE 2].** The ten functions that were already 100 in this bank stay 100.
- The report after those readers: `fuzzy_match_percent` 1.9755139, `matched_code` 5388, `matched_code_percent` 0.3549019, `matched_functions` 220 of 6978, `total_code` 1518166. Matched bytes did not change. `complete` stays false.
- Bank `0046CC50` now compiles. It is the S3D loader. `objdiff-cli diff` is 100 for `_fn_0046E1A0` (16 bytes, zeros 0x38 dwords), `_fn_0046DA20` (16), `_fn_0046DA30` (47), and `_fn_0046DA60` (31) under MSVC 6 `cl` 12.00.8804 `/O2`. **[STATE 3].** `_fn_0046E1B0` is 21.024271. It checks `S3DF`, then walks meshes at stride `0x54` using the count at `+0x20` and placed copies at stride `0x70` using the count at `+0x28`. The first count the header reader stores is the mesh count. The console registrations in the same bank (`test_meshes_on_load`, `load_lightmaps`, `p3d_files`, `lmap_reset`, `lmap_off`, and the `level` search path) are about 33. **[STATE 2].**
- The report after that loader: `fuzzy_match_percent` 2.0143943, `matched_code` 5498, `matched_code_percent` 0.36214748, `matched_functions` 224 of 6978, `total_code` 1518166. The new matched bytes are those four functions (110). `complete` stays false.
- Sixteen untouched banks now compile. The first pass on each is the console command the small function registers, the same shape as the commands in `0046CC50`. `objdiff-cli diff` is 33.055557 on `_fn_00463050` and the six beside it in `bank/00461DC0`. **[STATE 2].** The commands name the block: monster layout load and save (`0043F220`), character dumps (`00449B00`), summon and resist toggles (`00461DC0`), time and mesh scale (`0048A230`), phoenix and tiger riders (`004954A0`, `004989D0`), world map and random encounters (`004FCE40`). Matched bytes did not change.
- The report after those commands: `fuzzy_match_percent` 2.0573828, `matched_code` 5498, `matched_code_percent` 0.36214748, `matched_functions` 224 of 6978, `total_code` 1518166. `complete` stays false.
- The small functions beside those commands now have C. `objdiff-cli diff` is 100 for nineteen of them under MSVC 6 `cl` 12.00.8804 `/O2`. **[STATE 3].** `_fn_0043F400` and `_fn_0043F420` test a two-character prefix. `_fn_0043F440` tests for `<`. `_fn_0043F9C0` and `_fn_004D7B00` switch pages. `_fn_00440860` saves the current path and pushes another. `_fn_00444B20` and `_fn_00444B70` init the Joseph/Jekhar character pools. `_fn_00449B80` steps 0x3d4 bytes into a character record. `?fn_00462D20@Obj62D20@@QAEHXZ` and `?fn_00462D40@Obj62D20@@QAEHXZ` test bits on the linked actor. `?fn_004681D0@Obj681D0@@QAEXH@Z` stores one field. `_fn_00468DC0` plays a sound. `?fn_0048E440@Vec3@@QAEXPAU1@@Z` adds a vec3. `_fn_0049D970` increments the AI node counter. The character free list (`_fn_0044B280` 78.9, `_fn_0044B2B0` 56.7, `_fn_0044B380` 98.3), the flag tests, `_fn_0049A0A0` (94.0, index into six 0x20-byte tiger rows), and `_fn_004CA5C0` (98.6) are **[STATE 2].**
- `_fn_0048CF50` writes `+0xc0` when the object kind is 2. It is 99.64286. **[STATE 2].** Its callee stays `fn_0046B130`, with no leading underscore. An exportable name keeps the relocations in `bank/00473200`, and `objdiff-cli` then never returns. With the reloc dropped, `report generate` finishes in 0.050s.
- The report after those functions: `fuzzy_match_percent` 2.1106412, `matched_code` 5918, `matched_code_percent` 0.38981244, `matched_functions` 243 of 6978, `total_code` 1518166. `complete` stays false.
- `_fn_004FEE50@4` is the `_dlg.tbl` reader in `bank/004FCE40`. It appends `_dlg.tbl` to the directory it is given, opens that file, and walks `$Character:` blocks. Each block can carry `+Move in time:`, `+Move out time:`, and `+Flags:`, then a `+Topic:`. Under the topic the line tags are `+Shop`, `+Hidden`, the six speaker lines, `+Trigger:`, `+Event:`, the unhide and hide lines, `+AddXP:` (`AdjustXP`), `+AddSP:` (`AdjustSP`), `+AdjustGP:`, `+Gain item:`, `+Lose item:`, and the topic, quest, and hail tags. A `{` opens the block whose jump table sits past this symbol. `objdiff-cli diff` is 26.209. **[STATE 2].**
- The report after that reader: `fuzzy_match_percent` 2.1908257, `matched_code` 5918, `matched_code_percent` 0.38981244, `matched_functions` 243 of 6978, `total_code` 1518166. Matched bytes did not change. `complete` stays false.
- `_fn_0044B7A0` builds the item text at `0x8ec7c4`. The record's item kind is at `item+4`. Kind 2 compares the item name with the eight rings. Each ring reads its `*_ring_xp` value from the table at `0x2493a38`. Below 60000 it takes the uncharged line. Ring of Darkness tests `got_laharah_spirit` instead. Kind 0, kind 1, and the Joseph/Jekhar/Flece/Rosalind tail are still unread. `objdiff-cli diff` is 8.308. **[STATE 2].**
- The report after that text: `fuzzy_match_percent` 2.2322693, `matched_code` 5918, `matched_code_percent` 0.38981244, `matched_functions` 243 of 6978, `total_code` 1518166. Matched bytes did not change. `complete` stays false.
- `_fn_004FCE40` picks the random encounter. It writes the level name at `0x60a2c8` and the variant at `0x60a2e8`. Three samples choose region 0 (Iceland or the hills), region 1 (forest, tome pages 1 through 6), or region 2 (grassland). Buying page 7, holding page 8, and lacking `got_laharah_spirit` forces `rand-icelandnite01` before that split. `Eraekor Gemstone`, `ready_for_end`, `extra_1`, and `extra_5` select the other named variants. `objdiff-cli diff` is 23.792. **[STATE 2].**
- The report after that picker: `fuzzy_match_percent` 2.261073, `matched_code` 5918, `matched_code_percent` 0.38981244, `matched_functions` 243 of 6978, `total_code` 1518166. Matched bytes did not change. `complete` stays false.
- `_fn_0048A230` walks the entity list at `0x23059b8`. The link is `+0x98` and the sentinel is `0x2305920`. Kind `+0xa4` selects one of seventeen cases. Kind 9 attaches `spell-status.vfx` and arms the timer at `+0xdc` for `0x2710`. Kinds 1, 2, and 10 share the case that can attach `stone-fade.vfx`. Subtypes `0x1a`, `0x1d`, `0x1f`, `0x23`, and `0x4e` are queued, then the list at `0x8ecd78` is walked through `+0x318` up to `0x8eca60`. The other kind cases are still unread. `objdiff-cli diff` is 8.353. **[STATE 2].**
- The report after that walk: `fuzzy_match_percent` 2.2891014, `matched_code` 5918, `matched_code_percent` 0.38981244, `matched_functions` 243 of 6978, `total_code` 1518166. Matched bytes did not change. `complete` stays false.
- `_fn_0049FA70` looks up a path from a start point to a goal. Category indexes a 20-bit mask at `0x2452cb8`. A direct probe through `fn_004A55F0` can return the node. Otherwise it clears the cells from `0x2460dc0` to `0x246a050` (stride `0xa78`) and the open-node search is still unread. The success path walks the link at `node+0x1c`. `objdiff-cli diff` is 19.372. **[STATE 2].**
- The report after that query: `fuzzy_match_percent` 2.3132637, `matched_code` 5918, `matched_code_percent` 0.38981244, `matched_functions` 243 of 6978, `total_code` 1518166. Matched bytes did not change. `complete` stays false.
- `_fn_004FC4B0` is the other random-encounter picker. `got_ring_of_light` without `ghost_killed` forces `rand-forestnite1`. `got_ring_of_fire` without `phoenix_killed` forces `rand-hills01`. `got_ring_of_stone` without `serpent_killed` forces `rand-forest01`. Otherwise three samples choose the hills, the forest, or grassland, and `$hills001` plus `found_khosani` keep the hills on the day map. `objdiff-cli diff` is 14.407. **[STATE 2].**
- The report after that picker: `fuzzy_match_percent` 2.3285625, `matched_code` 5918, `matched_code_percent` 0.38981244, `matched_functions` 243 of 6978, `total_code` 1518166. Matched bytes did not change. `complete` stays false.
- `_fn_004A5750` tests two avoidance segments. A shared endpoint returns 3. A crossing returns 1, a crossing on an endpoint returns 4, and a miss returns 0. Parallel segments return -1. Collinear overlap returns 2 and writes one endpoint. The cross products are stored at `0x245f3a4`, `0x245f3a8`, and `0x245f3ac`. `objdiff-cli diff` is 29.722. **[STATE 2].**
- The report after that test: `fuzzy_match_percent` 2.351021, `matched_code` 5918, `matched_code_percent` 0.38981244, `matched_functions` 243 of 6978, `total_code` 1518166. Matched bytes did not change. `complete` stays false.
- `_fn_004A3890` draws the avoidance obstacles. Twenty layers live at `0x245f480`, indexed by layer plus page times `0x14`. The current layer is bright. The other layers draw only while `0x245cdf8` is set. A second list at `0x2460cb8` is drawn in red, then the `0xa78`-byte cells from `0x2460db4` draw their links when the cell layer and page match. The midpoint tick on each cell link is still unread. `objdiff-cli diff` is 33.468. **[STATE 2].**
- The report after that draw: `fuzzy_match_percent` 2.380843, `matched_code` 5918, `matched_code_percent` 0.38981244, `matched_functions` 243 of 6978, `total_code` 1518166. Matched bytes did not change. `complete` stays false.
- `APP_ICON` is the `.rsrc` section, 2432 bytes at file offset `0x1B3000`. The section match is 100. The report `matched_data` is 2432 of 215972, `matched_data_percent` 1.1260719. Code measures did not change. `complete` stays false.
- `?fn_00464100@Obj64100@@QAEXPAUChanceRecord@@D@Z` splits a hit across three samples. The actor samples at kinds 9 and `0xb` are clamped up to 0. The kind at `record+0x3c` indexes the byte map at `0x4645A0` when it is in `0x0a`..`0x55`. A roll from `fn_005426E0` is compared with the shares. The byte at `0x94173d` and actor field `+0xc == 7` forces that roll to 1. `objdiff-cli diff` is 46.156. **[STATE 2].**
- The report after that split: `fuzzy_match_percent` 2.4157107, `matched_code` 5918, `matched_code_percent` 0.38981244, `matched_functions` 243 of 6978, `total_code` 1518166, `matched_data` 2432, `matched_data_percent` 1.1260719. Matched code bytes did not change. `complete` stays false.
- Do not edit `src/` to force the whole tree to compile. Comment-only `.cpp` files stay out of `compiled_units()`.
- Do not commit `src/Engine/handler_4e68e0.cpp`. The names in it were invented.
- Do not set match percents in `tools/progress.py` or `tools/scrub_report.py`. objdiff's `matched_data_percent: 100` on a unit with `total_data` 0 is not a match; the scrub removes those keys.
- `fn_004E6890` size `0x66D0` is a bad split (jump table). Code in that span starts at `0x4E68E0`.
