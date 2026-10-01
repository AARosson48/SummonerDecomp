#### E. Asset Runtime & Symlink Directive
Do NOT instruct the workspace configuration to extract or duplicate game assets, movie files, maps, or audio packs into the workspace.
1. The repository treats the retail installation directory as a read-only static source.
2. The compilation output toolchain targets the 'build/' folder. 
3. A symbolic link configuration layer maps the retail asset files and middleware DLLs directly into 'build/' at runtime. This allows live debugging and binary execution checks without modifying or bloating the local Git repository tree.
