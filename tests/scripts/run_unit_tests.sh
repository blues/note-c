#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_SRC_DIR="$SCRIPT_DIR/../.."

if [ ! -f "$ROOT_SRC_DIR/CMakeLists.txt" ]; then
    echo "Failed to find note-c root directory. (did the location of run_unit_tests.sh change?)"
    exit 1
fi

pushd $ROOT_SRC_DIR $@ > /dev/null

cmake -B build/
if [ $? != 0 ]; then
    echo "Failed to run CMake."
    popd
    exit 1
fi

cmake --build build/
if [ $? != 0 ]; then
    echo "Failed to build code."
    popd
    exit 1
fi

ctest --test-dir build/ --output-on-failure
if [ $? != 0 ]; then
    echo "ctest failed."
    popd
    exit 1
fi

echo "Tests passed."
popd $@ > /dev/null 
