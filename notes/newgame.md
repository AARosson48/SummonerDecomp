# New game, the opening, and the level table

Read from `Sum.exe`. The New Game button is not its own symbol. It is the code at `0x41DAC0`, in the gap after `_fn_0041D8B0`.

## New Game

`0x41DAC0` calls `fn_00431A40` with the string at `0x59F1E8` and the dword at `0x2538A10`. The string is `masad`. The dword is a second name. At startup that memory is clear, so the second argument is null.

`fn_00431A40` looks the first name up in the level table and copies it to `0x60A2C8`. The second buffer, `0x60A2E8`, is the variant:

- A non-null second argument is copied there.
- A null argument uses the first of seven pointers at `0x25388C0 + id * 0x70` that is set. Those pointers are 0x10 bytes apart.
- If none is set, the variant is the same string as the level name.

It then requests mode 2 through `fn_00401020(2)`. If the current mode is 0, it also sets the byte at `0x5FBF18`.

The byte at `0x25443A8` is set when the string at `0x2494A9C` (`0x2493A38 + 0x1064`) is not empty, or when the lookup returns 3 and the variant is `masad`. Record 3 is `masad`, so a new game sets that byte.

## Modes

`fn_00401070` runs the mode. Each mode is three callbacks at `0x588A70`: enter, tick, leave. A request is applied on the next pass: leave the old mode, enter the new one, then the tick runs.

| Mode | Enter | Tick | Leave | Role |
| --- | --- | --- | --- | --- |
| 1 | `fn_00431C60` | `fn_00431E00` | `fn_00431DF0` | Front end. Enter calls `fn_00412620(0, 0xE)`, the title page |
| 2 | `fn_00431E20` | `fn_00431E70` | `fn_004F13E0` | One transition into a level. The tick requests mode 4 |
| 4 | `fn_00438130` | `fn_00438F30` | `fn_00438C80` | The level |

Mode 2's enter plays `"Title"` through `fn_004696D0` / `fn_00469080`, calls `fn_0046B200`, then `_fn_0046B680`. That store is the new-game wallet: gold `0x971868` becomes 30 and the count at `0x97186C` becomes 0. It then calls `fn_00439800`, which is `fn_004D4160` followed by the jump at `0x44B250`. `fn_004D4160` reads `level_items.tbl` (`$Name:` records until `#End`). `0x44B250` walks `dword [0x8EC7C0]` entries of 0x100 bytes at `0x8CBF1C` and sets or clears bit 0 from bit 3.

## Level table

`fn_004D6300` is the name lookup. Records are 0x40 bytes from `0x59F128` through `0x59FDE8`, 51 of them. The name pointer is the first field. `strcmp` is `0x57AE80`.

| Offset | Field |
| --- | --- |
| `0x00` | Internal name |
| `0x04` | First callback |
| `0x08` | Second callback |
| `0x0C` | Third callback |
| `0x10` | Fourth callback |
| `0x14` | Fifth callback, often 0 |
| `0x18` | Sixth callback |
| `0x1C` | A number. Masad's is `0xFA0` |
| `0x20` | Display name |

`fn_004F13E0` fills the unused slots on most records. Masad is index 3:

| Offset | Value |
| --- | --- |
| `0x00` | `masad` |
| `0x04` | `fn_004FD750` |
| `0x08` | `fn_004FD980` |
| `0x0C` | `fn_004FDC70` |
| `0x10` | `fn_004FDC90` |
| `0x18` | `fn_004FDD60` |
| `0x20` | `Masad` |

The other internal names, in order: Wolong, Catacombs, test, masad, worldmap1, lenele1b, lenele1c, lenele1d, lenele1e, sewer, rand-hills01, IkaemosBottomInt, IkaemosTopInt, IkaemosExt, KhosaniLab, IonaExt, WolongCaverns, TempleInt, rand-forest01, rand-forestnite1, Liangshan, Rand-Desert, IonaExt02, KhosaniStrng, LPalaceInt, tancredhouse, LPalaceInt02, KhosaniLab2, Wolong2, eleh, lenele2aa, lenele2ab, lenele3a, lenele3d, jadetemple, lenele1aa, lenele1ab, TempleInt2, IkaemosExt2, IkaemosBottomInt2, Rand-HillsNite01, Rand-Iceland01, Rand-Grassland01, Rand-Orenia01, Rand-OreniaNite01, Rand-DesertNite01, Rand-GrasslandNite01, Rand-IcelandNite01, endgame, lenele2d, sewerboss.

`endgame` has no callbacks. `eleh` is index 29. Its first callback is `fn_004F1490`. When `0x25443A8` is clear, that function passes `Joseph`, `Jekhar`, and `Rosalind` to `fn_00433F40`. It also starts the quest `Find Joseph` and calls through `worldmap1` and `Portcullis` when the flag `found_joseph` is clear.

## Opening scene

`fn_004FD980` is masad's second callback. It compares the variant at `0x60A2E8` with `masad_v2`. A match jumps to `fn_004F13E0` and the intro does not start. Any other variant calls `fn_004FD9B0`, `fn_004FDA20`, and `fn_004FDA90`, then jumps to `0x4FDB80`.

`fn_004FD9B0` sets the flag `masad_flag_gathering` on the object at `0x2493A38`.

`fn_004FDA90` returns immediately when `masad_intro` is already set. Otherwise, if a character pointer is at `0x5FCC88`, it gives that character a `Tunic`. It sets `masad_intro`, then starts the cutscene `Game-Pre-Intro` with completion function `0x4FDB10`.

`0x4FDB10` starts `Game-Intro`. Both names go through `fn_004CCF00`. `fn_004355E0` is the reader for `cutscene.tbl`.

`Game-Pre-Intro` in `cutscene.tbl`:

- Level `masad`
- Camera `PRE-INTRO-CAM.csc`
- Animation `Nar-PRE-INTRO-camera.vfx`
- Soundtrack `pre-intro_final.wav`
- Fog 50 to 70, fade in over 4 seconds, fade out 4 to 4

`Game-Intro` in the same file, with `+HideUnscripted`:

- Level `masad`
- Camera `Game-INTRO-CAM.csc`
- Animations `gi-body-cart.vfx`, `gi-thq-camera02.vfx`, `gi-Volition-camera04.vfx`, `gi-tigr.vfx`, `gi-body-carry-09.vfx`, `gi-title-camera10.vfx`
- Character animation `CS#WarHorse1#3` with `gi-SERP.vfx`
- Soundtrack `intro-final.wav`
- Visibility sets `$vis-gi-02` through `$vis-gi-10`
- Fog 50/70, then 30/50, then 40/60

`Game-Intro_cscript.tbl` is the camera path on that level. `$Level` is `masad`. The points are `$RIDE-A01` and the rest of that ride, each `$Type: "cutscene"`. `fn_004DB420` reads `$Name`, `$Type`, `+Plane`, `$Position`, and `$Orientation`. `fn_004CD5F0` reads the `#Navpoints` section. `fn_004CB7B0` reads `masad.tbl`: fog, ambient sounds, shopkeepers, effects, and the automap.

`fn_0042E9D0` is the `masad_intro` / `masad_basic_controls_tutorial` check. It has no direct call.

## Level enter

Mode 4's enter, `fn_00438130`, reads the previous mode and indexes a byte at `0x438560`. The byte selects one of three blocks at `0x438554`. Previous mode 2, which is what a new game just left, selects the block at `0x4381D6`. That block joins the setup at `0x438172`: the clock at `0x60A654` is cleared, then `fn_00432E50`, `fn_00487750`, `fn_00413370`, `fn_0046B210`, `fn_00489790`, `fn_0048A190`, `fn_0047A770`, `fn_00492B60`, `fn_004A34D0`, and `fn_00438570`.

`fn_00438570` returning 0 leaves the level. Bit 1 of `0x5FBFD8` clear also requests mode 1. Bit 1 set calls `fn_004814E0(4, 0)` and returns through the same cleanup, `fn_00413390` and `fn_004328A0`.

A nonzero return stays in the level. It calls `fn_0042FC60`, zeros `0x60A328`, and clears the byte at `0x5FCCB4`. An empty string at `0x2494A9C` calls `fn_004D3740`. A string that is set calls `fn_004D38D0`, and a zero from that function requests mode 1 and returns. The path that stays then calls `fn_004CC7E0`.

Bit 0 of `0x5FBFD8` with an item count of 0 gives a `Health Tonic` through `fn_0046BB00`. Mode 2's enter sets that bit. The byte at `0x5FBF19` replaces the 30 gold with `0x98961C`. The byte at `0x5FBF1A` is a second kit: gold 100, then `katana`, `wakasashi`, `icy dagger`, `sword of spirits`, `heavy boots`, `steel gauntlets`, `studded boots`, `studded leather`, `studded pants`, `Ring of Fire`, and `Ring of Darkness`. A clear byte skips that kit. The save from a new game had gold 30 and one stack of 3, which is the tonic path with both of those bytes clear.

Both paths meet at `0x43845D`. If the level id at `0x59F120` is not -1, `fn_004DAEF0` is called with the name at `0x60A2C8` and the variant at `0x60A2E8`. That function calls `fn_004D6340` and, when the slot at `0x25388C4` is positive, `fn_004D82F0` on the object at `0x25461A0`. `fn_00437130` runs only when bit 4 of `0x5FBFD8` is set. A new game from the title does not set that bit. `fn_004CCF00`, the cutscene start, calls `fn_004361D0` itself. `fn_004361D0` compares the current name with `any` and `Placeholder`, then calls `fn_00436620`, `fn_00448000`, `fn_0048D720`, `fn_00486C20`, `fn_004D64F0`, `fn_004353E0`, `fn_004ED0F0`, and `fn_00473200`. `fn_004ED0F0` is the cutscene-navpoint count.

`level_mount_archives` mounts three packfiles, in order: `levelm.vpp`, `levele.vpp`, `levelt.vpp`. It skips the mount when `eleh.s3d` is already on the level-model path.

| Archive | What is in it | Masad |
| --- | --- | --- |
| `levelm.vpp` | 60 `.s3d` files. The header is `S3DF`. This is the 3D map | `masad.s3d`, 8,070,034 bytes |
| `levele.vpp` | 60 `.pfg`, 50 `.lcf`, 37 `.vis` | `masad.pfg` is 767,196 bytes. `masad.lcf` and `masad.vis` are short text |
| `levelt.vpp` | 4,748 `.tga`, 20 `.vbm`, and six `.bat` mip scripts. These are the textures | `Masad-Cobble01.tga`, the grass and dirt sheets, the house and tavern walls, `MasadSky1-1.tga` |

`levelt` is not the table archive. `masad.tbl`, `masad_script.tbl`, `masad_v2_script.tbl`, `masad_dlg.tbl`, `masad_surfaces.tbl`, and `masad.eax` are in `tables.vpp`.

`masad.lcf` is a list of static pieces (`tree`, `woodpile`, `barrel`, `cart`, `watermill`, `masad-house01-01`) plus `nocollide:` lines and two `+script:` lines, `masad_script.tbl` and `masad_v2_script.tbl`. The search path in `vfs_init_search_paths` names `.lkf`. The entry inside `levele.vpp` is `.lcf`. `masad.vis` is two `$vis_set` lines and `$end`.

The same three archives hold the other campaign maps, plus `PCMS-boss1` through `PCMS-boss6`, `nohand`, and `E3-Ionaext02`. The PCMS maps have an `.s3d` and a `.pfg` and no `.lcf` or `.vis`.

The loose search paths those files are opened through are `data\models\levels` (`.v3d`, `.s3d`, `.vfx`, `.vlm`), `data\maps\levels` (`.vbm`, `.tga`, `.m2v`), `data\tables\levels` (`.tbl`), and `data\internal` (`.pfg`, `.mlo`, `.vis`, `.vex`, `.bsp`, `.lkf`, `.eax`).

Mode 4's tick, `fn_00438F30`, is the per-frame function. Its body is still closed.

## Live new game

The first watch was started with `CreateProcess` from this session, so Windows did not give it the foreground. The videos and the framebuffer stayed blank while the mode changes still happened. The run below was started by the player, with the watch already waiting. The player skipped the startup logos with Esc, clicked New Game, let the opening play through to gameplay, clicked through the tutorial screens, then used Esc, Quit to Menu, Yes, and Quit Game.

Page 9 is the pause popup. Leaving page 9 calls `0x4217B0`, showing it calls `0x4216F0`, and its frame calls `0x421CF0`. Those three use the slots built by `fn_00421590`, from `0x5C3B80` through `0x5C3C58`. The tutorial screens did not change the page number.

| Time | Player | What the watch saw |
| --- | --- | --- |
| 0s | Game started | Mode 0. The build-date gate at `0x5930B8` is 1 |
| 1.6s | Esc skipped the startup logos | Mode 1, page 14, gate cleared. Title |
| 8.4s | New Game | Mode 2, previous mode 1. Name and variant `masad`, intro byte 1, gold 30, request already 4, request flag still 1 |
| 8.6s | | Mode 4, previous mode 2. The request flag is clear |
| 8.8s | | Level id 3, page 0, items 1, flags `0x201`, a character pointer is set |
| 9.2s | Opening plays | Flags become `0x221`. The page stays 0 |
| 109.8s | | Flags become `0x8201` for one sample, then return to `0x221` |
| 206.9s | Gameplay, then the tutorial clicks | Flags become `0x205`, then `0x201`. The page is still 0 |
| 212.3s | Esc | Page 9, flags `0x205` |
| 215.5s | Quit to Menu, Yes | Mode 1, previous mode 4, page 14. The character pointer and the item count clear. Gold stays 30. The name is still `masad` |
| 216.7s | Quit Game | Mode 3. Both name buffers clear |
| 217.3s | | Process exited |

A second run loaded a save from the same place, after the tutorial. The player opened page 8 from the title, and the level came up from that page. Mode 2 was not sampled. Previous mode was already 2 when mode 4 appeared.

| Time | What the watch saw |
| --- | --- |
| 0s | Mode 0. The build-date gate is 1 |
| 1.8s | Mode 1, page 14, gate cleared |
| 8.4s | Page 8, still mode 1 |
| 12.8s | Mode 4, previous mode 2, page 8. Name and variant `masad`, intro byte 1, gold 30, items 0 |
| 13.2s | Level id 3, page 0, items 1, flags `0x201`, a character pointer is set |
| 19.5s | Page 9, flags `0x205` |
| 24.3s | Mode 1, page 14. The character pointer and the item count clear. Gold stays 30 |
| 26.3s | Mode 3. Both name buffers clear |
| 26.7s | Process exited |

The save never set flags `0x221`, and it never showed the one-sample `0x8201` value. Those two values belong to the opening. Gameplay in both runs is page 0, level id 3, flags `0x201`, gold 30, and one item. Flags `0x205` while the page is still 0 was the tutorial clicks. The same flags with page 9 are the pause popup. |
