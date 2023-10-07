#!/usr/bin/env bash
##
## EPITECH PROJECT, 2023
## coconut
## File description:
## entrypoint script
##

if [ $# -ne 2 -a $# -ne 3 ]; then
    echo "Usage: ./coding-style.sh <delivery_dir> <reports_dir> [copy_dest]"
    exit 84
fi

DELIVERY_DIR=$1
REPORTS_DIR=$2
COPY_DEST=${3:-$DELIVERY_DIR}

if [ "$DELIVERY_DIR" = "/mnt/delivery" -a -z "$3" ]; then
    mkdir -p /mnt/build
    COPY_DEST=/mnt/build
fi

if [ ! -d "$DELIVERY_DIR" ]; then
    echo "Error: $DELIVERY_DIR: No such directory"
    exit 84
fi

if [ ! -d "$REPORTS_DIR" ]; then
    echo "Error: $REPORTS_DIR: No such directory"
    exit 84
fi

if [ ! -d "$COPY_DEST" ]; then
    echo "Error: $COPY_DEST: No such directory"
    exit 84
fi

# In build directory
cd "$COPY_DEST"

# Copy from delivery directory to build directory
if [ "$DELIVERY_DIR" != "$COPY_DEST" ]; then
    cp -a "$DELIVERY_DIR"/. .
fi

# Back to root directory
cd - >/dev/null

find "$COPY_DEST" \
     \( -path "$COPY_DEST"/bonus \
     -o -path "$COPY_DEST"/tests \) -prune \
     -o \( -name '*.c' -o -name '*.h' \) -print0 \
     > "$COPY_DEST/coding-style-files.log"

# Back to build directory
cd - >/dev/null

# If the student has compile flags, ignore them
rm -f compile_commands.json
rm -f compile_flags.txt

if [ -d rush-1-1 ]; then
    # Rush1 is a special case
    bear -- gcc rush-1-*/*.c >/dev/null 2>&1
elif [ -f Makefile || -f makefile || -f GNUmakefile ]; then
    # Normal case, with a Makefile at the root
    bear -- make re >/dev/null 2>&1
else
    # Most days of the CPool
    if [ -d lib/my ]; then
        (cd lib/my && bear -- ./build.sh) >/dev/null 2>&1
    fi
    bear -- gcc *.c -I./include >/dev/null 2>&1
fi

# Back to root directory
cd - >/dev/null

REPORT="$REPORTS_DIR"/coding-style-reports.log.unsorted

cat "$COPY_DEST/coding-style-files.log" \
    | xargs -0 -n 1 coconut -p "$COPY_DEST" -o "$REPORT"

sort -V "$REPORT" > "$REPORTS_DIR"/coding-style-reports.log

rm -f "$REPORT"
