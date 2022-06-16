#!/bin/bash

mkdir -p build
pushd build

export CONAN_SYSREQUIRES_MODE=enabled
conan install .. --build=missing
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .