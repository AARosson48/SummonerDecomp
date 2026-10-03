# Assets

Retail archives, movies, and audio stay in the Steam or GOG install. Do not copy `.vpp` files, movies, or audio into this repository. `tools/extract_game.py` copies `Sum.exe` to `orig/sum-pc/Sum.exe` and can symlink the install into `build/` and `assets/` for a local run. Those paths are gitignored.

`binkw32.dll` and `eax.dll` are third-party middleware. Treat their imports as stubs. Do not decompile those DLLs.
