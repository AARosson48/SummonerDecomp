#!/usr/bin/env python3
"""Merge the decomp-toolkit function list with the names we already have.

decomp-toolkit's analyzer writes every function it finds to config/dtk_symbols.txt.
This script keeps those addresses, applies names from symbols.txt and
undefined_syms.txt, and gives every remaining .text byte an address too.
"""

import os
import re

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
TEXT_LO = 0x401000
TEXT_HI = 0x401000 + 0x17CB40

DTK_LINE = re.compile(
    r"(\S+) = \.(\w+):0x([0-9A-Fa-f]+); // type:(\w+)(?: size:0x([0-9A-Fa-f]+))?(.*)"
)


def _named(path):
    found = {}
    if not os.path.isfile(path):
        return found
    with open(path, "r", encoding="utf-8") as handle:
        for raw in handle:
            line = raw.split("#", 1)[0].strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) < 3:
                continue
            found[int(parts[0], 16)] = (parts[2], int(parts[1], 16))
    return found


def load_named():
    named = _named(os.path.join(ROOT, "config", "symbols.txt"))
    named.update(_named(os.path.join(ROOT, "config", "undefined_syms.txt")))
    return named


def load_dtk():
    path = os.path.join(ROOT, "config", "dtk_symbols.txt")
    text = []
    other = []
    with open(path, "r", encoding="utf-8") as handle:
        for raw in handle:
            line = raw.strip()
            match = DTK_LINE.match(line)
            if not match:
                continue
            name, section, address, kind, size, tail = match.groups()
            address = int(address, 16)
            size = int(size, 16) if size else 0
            if section == "text" and kind in ("function", "label"):
                text.append((address, size, name))
            else:
                other.append((name, section, address, kind, size, tail))
    return text, other


def build_text(named, dtk_text):
    by_addr = {}
    for address, size, name in dtk_text:
        by_addr[address] = (size, name)
    for address, (name, size) in named.items():
        if not (TEXT_LO <= address < TEXT_HI):
            continue
        current = by_addr.get(address)
        if current is None:
            by_addr[address] = (size, name)
        else:
            by_addr[address] = (current[0] or size, name)

    ordered = sorted(by_addr)
    emitted = []
    cursor = TEXT_LO
    for index, address in enumerate(ordered):
        if address > cursor:
            emitted.append((cursor, address - cursor, f"fn_{cursor:08X}"))
        preferred, name = by_addr[address]
        nxt = ordered[index + 1] if index + 1 < len(ordered) else TEXT_HI
        span = nxt - address
        size = preferred if 0 < preferred <= span else span
        if size <= 0:
            continue
        emitted.append((address, size, name))
        cursor = address + size
    if cursor < TEXT_HI:
        emitted.append((cursor, TEXT_HI - cursor, f"fn_{cursor:08X}"))
    return emitted


def format_line(name, section, address, kind, size, tail=""):
    size_text = f" size:0x{size:X}" if size else ""
    extra = tail or ""
    return f"{name} = .{section}:0x{address:08X}; // type:{kind}{size_text}{extra}"


def write_symbols():
    named = load_named()
    dtk_text, other = load_dtk()
    text = build_text(named, dtk_text)
    lines = [
        format_line(name, "text", address, "function", size)
        for address, size, name in text
    ]
    seen = {(section, address) for _, section, address, _, _, _ in other}
    for name, section, address, kind, size, tail in other:
        lines.append(format_line(name, section, address, kind, size, tail))
    for address, (name, size) in sorted(named.items()):
        if TEXT_LO <= address < TEXT_HI:
            continue
        if any(addr == address for _, addr in seen):
            continue
        section = "data"
        kind = "object"
        lines.append(format_line(name, section, address, kind, size))
    destination = os.path.join(ROOT, "config", "symbols_all.txt")
    with open(destination, "w", encoding="utf-8", newline="\n") as handle:
        handle.write("// Every .text address. Names come from symbols.txt when we have them.\n")
        handle.write("// fn_XXXXXXXX entries are functions decomp-toolkit found, or a gap it left.\n")
        for line in lines:
            handle.write(line + "\n")
    return len(text), sum(size for _, size, _ in text)


def main():
    count, covered = write_symbols()
    print(f"wrote config/symbols_all.txt ({count} .text symbols, {covered} bytes)")


if __name__ == "__main__":
    main()
