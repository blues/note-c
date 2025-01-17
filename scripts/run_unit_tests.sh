#!/bin/bash

COVERAGE=0
MEM_CHECK=0
LOW_MEM=0
SHOW_MALLOC=0
SINGLE_PRECISION=0
VERBOSE=0

while [[ "$#" -gt 0 ]]; do
    case $1 in
        --coverage) COVERAGE=1 ;;
        --low-mem) LOW_MEM=1 ;;
        --mem-check) MEM_CHECK=1 ;;
        --show-malloc) SHOW_MALLOC=1 ;;
        --single-precision) SINGLE_PRECISION=1 ;;
        --verbose) VERBOSE=1 ;;
        *) echo "Unknown parameter: $1"; exit 1 ;;
    esac
    shift
done

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_SRC_DIR="$SCRIPT_DIR/.."

if [[ ! -f "$ROOT_SRC_DIR/CMakeLists.txt" ]]; then
    echo "Failed to find note-c root directory. (did the location of run_unit_tests.sh change?)"
    exit 1
fi

pushd $ROOT_SRC_DIR $@ > /dev/null

CMAKE_OPTIONS="-DNOTE_C_BUILD_TESTS=1"
BUILD_OPTIONS=""
CTEST_OPTIONS=""
if [[ $COVERAGE -eq 1 ]]; then
    CMAKE_OPTIONS="${CMAKE_OPTIONS} -DNOTE_C_COVERAGE=1"
    BUILD_OPTIONS="${BUILD_OPTIONS} coverage"
fi
if [[ $LOW_MEM -eq 1 ]]; then
    CMAKE_OPTIONS="${CMAKE_OPTIONS} -DNOTE_C_LOW_MEM=1"
fi
if [[ $MEM_CHECK -eq 1 ]]; then
    CMAKE_OPTIONS="${CMAKE_OPTIONS} -DNOTE_C_MEM_CHECK=1"
    CTEST_OPTIONS="${CTEST_OPTIONS} -T memcheck"

    # This fixes a problem when running valgrind in a Docker container when the
    # host machine is running Fedora. See https://stackoverflow.com/a/75293014.
    ulimit -n 1024
fi
if [[ $SHOW_MALLOC -eq 1 ]]; then
    CMAKE_OPTIONS="${CMAKE_OPTIONS} -DNOTE_C_SHOW_MALLOC=1"
fi
if [[ $SINGLE_PRECISION -eq 1 ]]; then
    CMAKE_OPTIONS="${CMAKE_OPTIONS} -DNOTE_C_TEST_SINGLE_PRECISION=1"
fi
if [[ $VERBOSE -eq 1 ]]; then
    CMAKE_OPTIONS="${CMAKE_OPTIONS} -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON --log-level=VERBOSE"
fi

cmake -B build/ $CMAKE_OPTIONS
if [[ $? -ne 0 ]]; then
    echo "Failed to run CMake."
    popd $@ > /dev/null
    exit 1
fi

cmake --build build/ -- $BUILD_OPTIONS -j
if [[ $? -ne 0 ]]; then
    echo "Failed to build code."
    popd $@ > /dev/null
    exit 1
fi

ctest --test-dir build/ --output-on-failure ${CTEST_OPTIONS}
if [[ $? -ne 0 ]]; then
    echo "ctest failed."
    popd $@ > /dev/null
    exit 1
fi

echo "Tests passed."
popd $@ > /dev/null
