#!/usr/bin/env bash
##
## EPITECH PROJECT, 2023
## coconut
## File description:
## functional tests
##

REPORT_FILE="coding-style-reports.log"
EXPECT_FILE="expected-reports.log"

if [ "$1" = "--local" ]; then
    echo "Building locally"
    cmake -Bbuild
    cmake --build build -j
    echo "Installing locally"
    sudo cmake --install build
else
    echo "Building docker image"
    IMAGE=$(docker build -q .)
    echo "Built image: $IMAGE"
fi

error=0

for expfile in ./tests/*.t/"$EXPECT_FILE"; do
    dir=$(dirname "$expfile")
    printf "\033[33mRunning test $dir\033[0m\n"
    dirname=$(realpath "$dir")
    if [ "$1" = "--local" ]; then
        script=$(realpath "./check.sh")
        pushd "$dirname" > /dev/null
        rm -f "$REPORT_FILE"
        "$script" . .
        sed -i -e 's#^'"$dirname"'/##' "$REPORT_FILE"
        make fclean > /dev/null
        popd > /dev/null
    else
        docker run --rm -i \
           -v "$dirname":/mnt/delivery \
           -v "$dirname":/mnt/reports \
           "$IMAGE" /mnt/delivery /mnt/reports
    fi

    if diff "$dir/$EXPECT_FILE" "$dir/$REPORT_FILE"; then
        printf "\033[32mTest $dir: OK\033[0m\n"
    else
        printf "\033[31mTest $dir: KO\033[0m\n"
        error=1
    fi
done

exit $error
