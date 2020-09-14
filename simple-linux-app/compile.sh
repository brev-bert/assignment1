#!/bin/bash

# clean project
rm -rf ./bin ./build

# run cmake
cmake -S ./ -B ./build

# run make
cd ./build
make
cd ..
