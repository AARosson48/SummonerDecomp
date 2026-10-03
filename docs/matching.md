# Matching a function

[![Code](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc)
[![Data](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc.svg?mode=shield&measure=data&label=Data)](https://decomp.dev/AARosson48/SummonerDecomp/sum-pc)

Those shields are the project percent. They update from `build/report.json` after CI uploads the `sum-pc_report` artifact. **Matched code** is the bytes in functions objdiff scores at 100%. **Fuzzy** is the average similarity, including partial functions, so it is higher. A function under 100% stays in the tree when it builds. It is not a match, and it does not add matched bytes.

`// FUNCTION: SUM 0x........` marks a 100% match. Anything short of that stays `// STUB:`.

The full link is a separate problem. `lld-link` produces an executable the same size as retail, identical from file offset `0x2a0` to the end. The DOS stub, Rich header, timestamp, and section table differ, so `config/sum-pc/build.sha1` fails. Function matching does not wait on that. `notes/build.md` is the log.

## Tools already used

Cursor's coding agent drafted most of the C, then each function was compiled with `cl` 12.00.8804 `/O2` and scored with objdiff. Binary Ninja's decompiler was a reading aid on `vsdk/geom/effect_mem.cpp`, `Engine/ai/pathfinding.cpp`, `levelscripts/conversation.cpp`, `Engine/rendering/levelrender.cpp`, and `bank/00482850`, among others. Capstone was used when a register or a compare did not match.

Binary Ninja drops float code, inverts some tests, and can print a `ret` immediate as a return value. Check the bytes.

If you use an agent or a decompiler, name it in the pull request.

## Score one function

```sh
ninja build/src/bank/00482850.obj
tools/bin/objdiff-cli.exe diff -1 build/base/obj/bank/00482850.o -2 build/src/bank/00482850.obj
```

The first file is the retail object from the split. The second is what `cl` just wrote. A bank target has no `.cpp` in the name. A named unit keeps it, for example `build/src/Engine/ai/pathfinding.cpp.obj`.

1. **Build failure.** `cl` returned non-zero. Fix the syntax and rebuild.
2. **Diff mismatch.** It compiled, and the instructions, stack, or registers differ. Change types, alignment, or calling convention and rebuild.
3. **Binary match.** objdiff reports 100%. Record the size, the compiler, and the flags in `notes/build.md`.

Do not edit `tools/progress.py` or `tools/scrub_report.py` to raise a percent. The diff is the check. A report fuzzy of 100 with mismatched rows in `objdiff diff` is still a mismatch.

## Symbols

`extern "C"` emits a leading underscore (`_fn_00486520`). Prefix the function you are defining so objdiff can pair it. Leave `fn_0046B130` unprefixed. Exporting that name makes objdiff follow a relocation and not finish.

Add a symbol only when an existing function ends at that address and the new size stops before the next function. An address stays `fn_` until a string in the executable proves a name. `thiscall` methods use the C++ mangled name so `cl` passes `this` in `ecx`.

## Where code goes

Assert strings still carry the original paths (`D:\projects\Summoner\pccode\...`). `config/modules.txt` is that list. `config/splits.txt` is the address range for each unit. A function goes in the original `.cpp` once an assert, a string cluster, or an address range places it there. `bank/00xxxxxx` is a slice of about 16KB whose original file is not known yet.

`configure.py` `compiled_units()` is the list `ninja` compiles. A `.cpp` that does not build with MSVC 6 stays off that list. Adding one that fails will fail CI.

`python configure.py` rewrites `config/symbols_all.txt` and `config/reccmp.csv`. Leave those out of a commit unless the symbol list is the change. `reccmp-project.yml` is the address list for [reccmp](https://github.com/isledecomp/reccmp). The decomp.dev score comes from objdiff. `reccmp-reccmp` needs a linked build and a PDB, which this tree does not have yet.

Summoner shares a Volition SDK layout with Red Faction. Reconstruct functions from `Sum.exe`. Do not paste another project's source in.

## What not to send

- `Sum.exe`, `orig/`, `build/`, or the `.vpp` archives.
- A linked `build/sum-pc/Sum.exe` produced with `/FORCE`.
- One report unit per function.
- `src/Engine/handler_4e68e0.cpp`. The names in that file were invented. The bytes at `0x4E68E0` and `0x4E6930` match, and they are not in the decomp.dev report.
