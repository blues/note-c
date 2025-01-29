#!/bin/bash
# Exit on error
set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
SRC_DIR="$SCRIPT_DIR/.."

# Ensure build directory exists and compile_commands.json is generated
if [ ! -d "$SRC_DIR/build" ] || [ ! -f "$SRC_DIR/build/compile_commands.json" ]; then
    cmake -B "$SRC_DIR/build" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
fi

# Ensure reports directory exists
mkdir -p "$SRC_DIR/reports"

# Run cppcheck with comprehensive checks
CPPCHECK_OUTPUT_FILE="$SRC_DIR/reports/cppcheck_results.xml"
cppcheck \
    --enable=warning \
    --error-exitcode=1 \
    --force \
    --inline-suppr \
    --project="${PWD}/build/compile_commands.json" \
    --suppress=missingIncludeSystem \
    --suppress=nullPointerRedundantCheck:*/n_cjson.c \
    --xml \
    --output-file="$CPPCHECK_OUTPUT_FILE"

# If we get here, cppcheck passed
echo "Static analysis complete. Results saved to cppcheck_results.xml"
