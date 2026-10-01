#!/usr/bin/env python3

def apply(config, args):
    config["baseimg"] = "retail_bin/Sum.exe"
    config["myimg"] = "build/Sum.exe"
    config["mapfile"] = "build/Sum.map"
    config["source_directories"] = ["src", "include"]
    config["arch"] = "x86"
    config["objdump_flags"] = ["-M", "intel"]
