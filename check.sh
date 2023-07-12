#!/usr/bin/env bash
##
## EPITECH PROJECT, 2023
## coconut
## File description:
## entrypoint script
##

if [ $# -ne 2 ]; then
    echo "Usage: ./coding-style.sh <delivery_dir> <reports_dir>"
    exit 84
fi

DELIVERY_DIR=$1
REPORTS_DIR=$2

if [ ! -d "$DELIVERY_DIR" ]; then
    echo "Error: $DELIVERY_DIR: No such directory"
    exit 84
fi

if [ ! -d "$REPORTS_DIR" ]; then
    echo "Error: $REPORTS_DIR: No such directory"
    exit 84
fi

pushd "$DELIVERY_DIR" > /dev/null || exit 84

bear -- make re >/dev/null 2>&1

popd >/dev/null

REPORT="$REPORTS_DIR"/coding-style-reports.log

rm -f "$REPORT"

find "$DELIVERY_DIR" \
     -path "$DELIVERY_DIR"/bonus \
     -o -path "$DELIVERY_DIR"/tests -prune \
     -o \( -name '*.c' -o -name '*.h' \) -print0 \
    | xargs -0 coconut -p "$DELIVERY_DIR" -o "$REPORT"
