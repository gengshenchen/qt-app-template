#!/bin/bash
set -e

DUMP_SYMS=../3rdparty/crashpad/tools/linux/dump_syms   
OUTPUT_DIR=./symbols

mkdir -p "$OUTPUT_DIR"

for binary in "$@"; do
    echo "Generating .sym for $binary..."

    sym_output=$($DUMP_SYMS "$binary")
    debug_id=$(echo "$sym_output" | head -n1 | awk '{print $4}')
    module_name=$(basename "$binary")

    sym_dir="$OUTPUT_DIR/$module_name/$debug_id"
    mkdir -p "$sym_dir"
    echo "$sym_output" > "$sym_dir/$module_name.sym"

    echo "→ Saved to $sym_dir/$module_name.sym"
done

