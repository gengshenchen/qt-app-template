#!/bin/bash
set -e

STACKWALK=../3rdparty/crashpad/tools/linux/minidump_stackwalk  
DUMP_FILE="$1"
SYMBOLS_DIR=./symbols

if [ ! -f "$DUMP_FILE" ]; then
    echo "❌ Dump file not found: $DUMP_FILE"
    exit 1
fi

$STACKWALK "$DUMP_FILE" "$SYMBOLS_DIR"

