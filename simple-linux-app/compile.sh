#!/bin/bash

usage () {
    echo "Usage:
    -G  Compile with GCC
    -C  Build and compile with cmake"
}

if [ $# -eq 0 ] ; then
    usage
    exit 0
else
    if [ "$1" = "-G" ] ; then
        # need g++, gcc doesn't work
        g++ ./src/main.cpp -o ./bin/SIMPLE_PROGRAM_ENV -std=c++11 -pthread
    elif [ "$1" = "-C" ] ; then

        # clean project
        rm -rf ./bin ./build

        # run cmake
        cmake -S ./ -B ./build

        # run make
        cd ./build
        make
        cd ..

    else
        usage
        exit 0
    fi
fi