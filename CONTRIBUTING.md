# Contributing

Build the tree with the steps in [README.md](README.md). The match rules, the tools already used, and what to leave out of a commit are in [docs/matching.md](docs/matching.md).

A pull request should include the objdiff percent and size for each function you touched, and the `cl` version. Name the agent or decompiler if one drafted the change. For a 100% function, add a line to `notes/build.md`.

A function under 100% can be submitted when it builds. Say the percent. Pushing `main` runs `.github/workflows/check.yml`, which compiles the tree and uploads `build/report.json`. That artifact is what decomp.dev displays.
