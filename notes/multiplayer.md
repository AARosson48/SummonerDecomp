# Multiplayer menu

The five builders are C and compile. They are not byte-matches. `_fn_004063F0` is character select, `_fn_004099D0` is create game, `_fn_0040A6A0` is the game list, `_fn_004191B0` is trade, and `_fn_0041BEE0` is level select.

Read from `Sum.exe` and from one live process at image base `0x400000`. The process was force-closed after the read. Language index `0x60AD68` was 1. Resolution index `0x5BD318` was 0. Every background below was 800×600. A pressed image shares the rectangle of the highlight image in front of it, and the click table ignores the pressed slot.

`fn_00412620(page, arg)` is the page switch. The page number is stored at `0x5BD814`. These pages are the multiplayer screens:

| Page | Screen | Show function |
| --- | --- | --- |
| `0x0E` | Title, including the Multi button | `fn_0041D8B0` |
| `0x0F` | `PXO-GameList` | `0x40A850` |
| `0x10` | `PXO-CharCreate` | `0x406590` |
| `0x11` | `PXO-CreateGame` | `0x409B10` |
| `0x12` | `PXO-LevelSelect` | `0x41C080` |
| `0x13` | `MultiTrade` | `0x419380` |

## Title button

`MainMultiHiLite` calls `fn_0041DAE0`. That function reads the net state at `0x270812C`.

- State 0, 1, or 2 opens a centered dialog through `fn_00413E40`. The strings come from globals at `0x25CE0F0` and its neighbors.
- Any other state, with the byte at `0x2710EA4` set, calls `fn_00401020(0xA)`.

Live, at about 1.5 seconds, the net state was 3 and `0x2710EA4` was 1. The title page was `0x0E` and the current mode was 1. Calling `fn_0041DAE0` set the requested mode to 10 and left the dialog buffers empty. Mode 10's enter function, `fn_0043F9C0`, calls `fn_00412620(0x10, 0)`, which shows `PXO-CharCreate`.

## Character select (`fn_004063F0`)

34 widgets at `0x5B6940`. Built during startup. Four checkboxes call `fn_00446BD0` with 0, 1, 2, and 3.

| Slot | Name | Rectangle | Click |
| --- | --- | --- | --- |
| 0 | `PXO-CharCreate` | 0, 0, 800×600 | none |
| 1 | checkbox | 648, 191, 19×19 | `fn_00446BD0(0)` |
| 3 | checkbox | 648, 218, 19×19 | `fn_00446BD0(1)` |
| 5 | checkbox | 648, 244, 19×19 | `fn_00446BD0(2)` |
| 7 | checkbox | 648, 270, 19×19 | `fn_00446BD0(3)` |
| 9 | `CharCreateDeleteHiLite` | 40, 493, 93×76 | dialog from `0x4078B0` |
| 11 | `CharCreateProfileHiLite` | 145, 493, 99×76 | dialog from `0x4079C0` |
| 13 | `CharCreateSelectHiLite` | 255, 493, 89×76 | pushes the string `Multiplayer Char`, then `fn_00401020(0xB)` (mode 11) |
| 15 | `GameListCreateHiLite` | 441, 493, 89×78 | dialog through `fn_00413E40` |
| 17 | `CharCreateCancelHiLite` | 639, 525, 123×41 | `fn_00401020(1)` |
| 19 | `Profile-bkgrnd` | 213, 153, 375×296 | popup, not a button |
| 20 | `Icon-GrnClseHilite` | 534, 179, 31×28 | `fn_004074D0`, clears the byte at `0x5B6838` |
| 22 | arrow up | 329, 56, 20×28 | scroll |
| 24 | arrow down | 329, 451, 20×28 | scroll |
| 26 | arrow up | 738, 354, 20×28 | scroll |
| 28 | arrow down | 738, 431, 20×28 | scroll |
| 30 | arrow up | 551, 223, 20×28 | scroll |
| 32 | arrow down | 551, 403, 20×28 | scroll |

The highlight slot is the one in the table. The following press slot uses the same rectangle and has no action.

## Game list (`fn_0040A6A0`)

18 widgets at `0x5BC760`. Also built during startup. Create-game's Cancel requests mode 12, and mode 12 shows this page.

| Slot | Name | Rectangle | Click |
| --- | --- | --- | --- |
| 0 | `PXO-GameList` | 0, 0, 800×600 | none |
| 1 | arrow up | 768, 103, 20×28 | `fn_0040B340`, steps the list cursor at `0x5BC71C` |
| 3 | arrow down | 768, 470, 20×28 | `fn_0040B360` |
| 5 | `GameListRefreshHiLite` | 24, 511, 119×44 | `fn_0040B390`, clears the list cursor |
| 7 | `GameListJoinHiLite` | 515, 511, 66×78 | `fn_00408540`, then `fn_00470BC0` |
| 9 | `GameListCreateHiLite` | 596, 511, 89×78 | dialog, or `fn_00401020(0xD)` (mode 13) when the net state is not 1 or 2 |
| 11 | `GameListChatHiLite` | 696, 511, 73×78 | `fn_00401020(0xB)` when the byte at `0xA59A88` has bit 0 set |
| 13 | `PXO-PasswordPopup` | 213, 218, 375×168 | popup |
| 14 | `Icon-CheckHilite` | 534, 330, 31×28 | same join call as slot 7 |
| 16 | `Icon-GrnClseHilite` | 534, 244, 31×28 | clears the password bytes at `0x5BC710` and `0x5BC728` |

## Create game (`fn_004099D0`)

23 widgets at `0x5BC498`. Built during startup. Shown as page `0x11`.

| Slot | Name | Rectangle | Click |
| --- | --- | --- | --- |
| 0 | `PXO-CreateGame` | 0, 0, 800×600 | none |
| 1 | arrow left | 154, 178, 29×16 | decrements the counter at `0x5BC490` while it is greater than 1 |
| 3 | arrow right | 240, 178, 29×16 | the matching increment |
| 5 | checkbox | 120, 300, 19×19 | `fn_00409C60` |
| 7 | checkbox | 231, 300, 19×19 | `fn_00409C60` |
| 9 | checkbox | 325, 300, 19×19 | `fn_00409C60` |
| 11 | arrow left | 73, 550, 29×16 | step |
| 13 | arrow right | 159, 550, 29×16 | step |
| 15 | arrow left | 218, 550, 29×16 | step |
| 17 | arrow right | 304, 550, 29×16 | step |
| 19 | `CreateCancelHiLite` | 371, 516, 223×58 | `fn_00401020(0xC)`, and mode 12 shows the game list |
| 21 | `CreateBeginHiLite` | 594, 516, 198×58 | `fn_0043EDC0`, then `fn_00401020(0xE)`. Mode 14 shows level select |

## Level select (`fn_0041BEE0`)

15 widgets at `0x5C1AE8`. Built during startup. Shown as page `0x12` after Create Begin.

| Slot | Name | Rectangle | Click |
| --- | --- | --- | --- |
| 0 | `PXO-LevelSelect` | 0, 0, 800×600 | none |
| 1 | `LevelSelectBackHiLite` | 27, 8, 181×45 | `fn_004814E0` |
| 3 | arrow up | 776, 25, 20×28 | scroll |
| 5 | arrow down | 776, 199, 20×28 | scroll |
| 7 | arrow up | 484, 320, 20×28 | scroll |
| 9 | arrow down | 484, 528, 20×28 | scroll |
| 11 | `LevelSelectKickHiLite` | 550, 465, 84×38 | `fn_0041CD10`, then `fn_0043EC40` |
| 13 | `CreateBeginHiLite` | 594, 516, 198×58 | `fn_0043EC40` |

## Trade (`fn_004191B0`)

28 widget names start at `0x58CDA0`, and the runtime slots are `0x5C1598`. This builder is not in the title-screen init. It runs from mode 4, through `fn_004118E0`. On the title screen the live slots were still zero, so there are no rectangles for it yet. Page `0x13` shows it.

The two `MultiTrade-Ready` images are in the table and the click switch does nothing for them. The buttons that do something are the arrow pairs, the `LevelSelectProfile` slots, `TradeGold-Bkgrnd`, and the two close icons. Profile slot 15 jumps to `fn_00471820`. The close path calls `fn_004715A0` and `fn_00471370`.
