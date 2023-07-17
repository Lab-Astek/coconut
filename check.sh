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

cd "$COPY_DEST"

if [ "$DELIVERY_DIR" != "$COPY_DEST" ]; then
    cp -a "$DELIVERY_DIR"/. .
fi

cd - >/dev/null

find "$COPY_DEST" \
     \( -path "$COPY_DEST"/bonus \
     -o -path "$COPY_DEST"/tests \) -prune \
     -o \( -name '*.c' -o -name '*.h' \) -print0 \
     > "$COPY_DEST/coding-style-files.log"

cd - >/dev/null

bear -- make re >/dev/null 2>&1

cd - >/dev/null

REPORT="$REPORTS_DIR"/coding-style-reports.log

rm -f "$REPORT"

cat "$COPY_DEST/coding-style-files.log" \
    | xargs -0 -n 1 coconut -p "$COPY_DEST" -o "$REPORT"
