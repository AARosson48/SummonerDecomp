# Master Technical Brief: Summoner (2001) PC Matching Decompilation Project

This document serves as the absolute, immutable source of truth for the workspace architecture, toolchain isolation, environment configurations, verified file extensions, and technical boundaries of the "Summoner" (2001) decompilation project.

---

## 1. Project Intent & Target Parameters
- **Target Title:** Summoner (PC Windows Version, March 2001 Release)
- **Developer/Publisher:** Volition / THQ
- **Target Executable:** `Sum.exe` (or `Summ.exe` depending on specific retail patch revision)
- **Engine Framework:** Volition CTG (Core Technology Group) Engine Framework
- **Platform Architecture:** Windows x86 (32-bit Portable Executable binary format)
- **Original Compiler Profile:** Microsoft Visual C++ 6.0 (MSVC 6.0 / MSVCRT)
- **Primary Technical Milestone:** Reconstruct a 100% byte-matching clone of the runtime core, with an explicit emphasis on isolating the PC-exclusive DirectPlay 7/8 online multiplayer networking code subsystems.

---

## 2. Directory Architecture & Isolation Firewall
The project repository must maintain total isolation from the user's authentic game installation files. Heavy assets, maps, movies, and audio packs are never duplicated into the Git tree. Instead, local execution paths utilize system links (`symlinks`).

```text
├── .github/
│   └── workflows/
│       └── check.yml        # Automation checking matching bytes on every push
├── assets/                  # Symlinks mapped directly to original game asset packages
├── baserom/
│   └── baserom.exe          # Isolated copy of the target Sum.exe binary ONLY
├── build/                   # Compilation sandbox target folder (Ignored by Git)
│   ├── Sum.exe              # Generated matching executable artifact
│   └── Sum.map              # Linker map file required by decomp.dev / objdiff
├── tools/
│   ├── extract_game.py      # Automated registry scanner and environment setup tool
│   └── progress.py          # Telemetry parser reporting stats to decomp.dev
├── include/                 # Reconstructed C/C++ header files
│   ├── common.h
│   ├── network/             # DirectPlay packet & lobby structures
│   ├── vfs.h                # Read-only archive table definitions
│   └── engine/              # Engine sub-module trackers
├── src/                     # Decompiled C/C++ source code translation units
│   ├── main.c
│   ├── vfs.c                # Black-box file-seeking logic
│   └── network/             # Online multiplayer matching targets
├── configure.py             # Python script generating the Ninja build configuration
├── diff_settings.py         # Objective configuration bridge for objdiff
└── splat.yaml               # Binary splitting layout descriptor for slicing instructions
```

---

## 3. Toolchain & Platform Integration Rules

### A. decomp.dev Dashboard Hook Compatibility
Because no default upstream platform template exists for early Volition PC executables on decomp.dev, the build framework must follow these custom telemetry generation patterns:
1. The `make_full_disasm_for_code` flag must always be forced to `True` inside the `splat.yaml` options block.
2. The compilation pipeline must instruct the linker to generate an MSVC-style symbol map file (`build/Sum.map`).
3. The workspace must contain `tools/progress.py` to parse the map outputs and print the following exact JSON format to standard output:

```json
{
  "version": 1,
  "metrics": {
    "code": { "total": 524288, "matched": 0, "percentage": 0.0 },
    "data": { "total": 196608, "matched": 0, "percentage": 0.0 }
  }
}
```

### B. Middleware Boundary Isolation (Bink & EAX)
Do NOT attempt to analyze, slice, parse, or decompile `binkw32.dll` or `eax.dll`. These are independent third-party middleware components.
1. Treat all Bink Video and Creative EAX sound symbols found inside the `Sum.exe` structure strictly as external Import Address Table (IAT) function stubs.
2. Maintain clean, decoupled abstractions for these libraries. This preserves the architecture for future native recompilation projects (recomps), where legacy code can be dropped for modern alternatives (e.g., FFmpeg, SDL_mixer, OpenAL, FMOD).

### C. Runtime Asset Virtualization
1. The build system must treat the consumer's Steam or GOG directory as a read-only static reference source.
2. The environment initialization workflow automatically creates local symbolic links inside the `build/` directory pointing back to the game's data files. This permits live debugging and local matching tests without bloating the codebase repository.

---

## 4. Operational Boundaries & Tooling Scope
The scope of this repository is strictly limited to matching the compiled binary footprint of `Sum.exe`. It is completely decoupled from asset modification, graphic adjustments, or resource packaging.

### A. Non-Interference with .VPP Asset Archives
- Do NOT generate utilities, code scripts, or tool layouts to compress, pack, modify, or rebuild `.VPP` virtual package archives.
- The project explicitly treats all game assets as immutable, read-only data bundles.
- All file offset tables, byte lengths, data headers, and layout maps handled in the source code are used solely to let `Sum.exe` communicate seamlessly with the authentic files.

### B. Leveraging Existing Community Tooling
- Any asset verification, data ripping, or script dumping needed during reverse engineering should be performed using the pre-existing external Descent Manager Toolkit.
- The decompiled virtual file system code (`src/vfs.c`) only replicates the runtime streaming and index tracking logic of the original executable to verify matching binary alignments.

---

## 5. Verified Engine Archive Architecture & File Handlers
The Volition CTG Engine relies on specific, proprietary binary and text file structures mapped across the 11 `.VPP` virtual systems. The decompiled virtual file system layer (`src/vfs.c`) must be built around parsing and loading these exact file types:

### A. VPP Archive Content & Extension Breakdown
- **chars.vpp**: Actor assets, structural animations, and debugging script tools.
  - `.MVF` files (Proprietary **Motion Video/Skeletal Movement File** transformation tracks)
  - `.SVF` files (Proprietary Skeletal/Mesh Variation Variant format)
  - `.VIM` files (Proprietary **Volition Item/Mesh structure** utilized for structural overlays)
  - `.TGA` / `.VBM` files (Texture sheets and animation layouts)
  - `.BAT` files (Console batch testing strings)
  - `.SCC` source control stamps

- **summoner.vpp**: Holds core system configurations, fonts, and interface structures.
  - `.ARR` files (Array/Asset registries)
  - `.VF` files (Proprietary Font descriptors)
  - `.VBM` files (Animation descriptors/textures)
  - `.TGA` / `.BMP` files (Standard graphic/UI images)
  - `.VAF` files (Visual Asset/Audio index layer)
  - `.PSD` files (Original Adobe Photoshop project layers left in retail tree)
  - `.SCC` source control stamps

- **tables.vpp**: The central database repository of engine values.
  - `.TBL` files (Plaintext structural table registries containing scripts like `#Items`)
  - `.EAX` files (Environmental Audio configuration presets)

- **sounds.vpp** & **music.vpp**: Audio streaming assets.
  - `.WAV` files (Standard audio waveforms parsed by the audio mixer)

- **levelt.vpp**: Technical test maps and asset sandboxes.
  - `.TGA` / `.VBM` graphics
  - `.BAT` files (Internal console batch command scripts)
  - `.SCC` source control stamps

- **levelm.vpp**: Multiplayer Level Definitions.
  - `.S3D` files (Proprietary **Summoner 3D Level geometry and map scripts** exclusive to multiplayer zones)

- **levele.vpp**: Main Game Environment Maps.
  - `.PFG` files (Proprietary Engine Geometry data)
  - `.VIS` files (Visibility/Culling data maps for renderer optimization)
  - `.LCF` files (Proprietary **Summoner Level Configuration** control blocks)

- **items.vpp**: Dynamic inventory items.
  - `.VIM` files (Proprietary Volition Item Mesh data structure)
  - `.TGA` graphics
  - `.SCC` source control stamps

- **effects.vpp**: Visual spell effects and particle rules.
  - `.RFX` files (Proprietary Render/Fx scripts)
  - `.TGA` / `.VBM` textures
  - `.SCC` source control stamps

- **cutscene.vpp**: Cinematic scenario timelines.
  - `.CSC` files (Proprietary **Cutscene Script Configuration** instructions)
  - `.MVF` files (Skeletal transformation tracks)
  - `.TGA` graphics, `.WAV` audio streams
  - `.SCC` source control stamps

### B. Engineering Impact on Decompilation Priorities
1. **Multiplayer Target Isolation:** To bring back multiplayer, your AI agent needs to focus heavily on how the executable reads `.S3D` files from `levelm.vpp`. It does *not* need to read `.PFG` or `.LCF` files from `levele.vpp`, as those are explicitly for the single-player worlds.
2. **Database Lookup Mapping:** The core system rules are handled by reading `.TBL` files inside `tables.vpp`. The AI can map out the engine's text parser precisely by tracing functions that read the parameters inside those files.
3. **Ignoring Source Control:** The presence of `.SCC` (Microsoft Visual SourceSafe) files across folders means the original build system accidentally dumped raw development metadata into the retail master. Your decompiled asset streaming code must be written to explicitly skip or ignore `.SCC` file headers to avoid buffer parsing exceptions.

---

## 6. Target Project File Templates

### A. File: `diff_settings.py`
```python
#!/usr/bin/env python3

def apply(config, args):
    config["baseimg"] = "baserom/baserom.exe"
    config["myimg"] = "build/Sum.exe"
    config["mapfile"] = "build/Sum.map"
    config["source_directories"] = ["src", "include"]
    config["arch"] = "x86"
    config["objdump_flags"] = ["-M", "intel"]
```
