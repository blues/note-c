#!/bin/bash
# Exit on error
set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
SRC_DIR="$SCRIPT_DIR/.."

# Ensure build directory exists and compile_commands.json is generated
if [ ! -d "$SRC_DIR/build" ] || [ ! -f "$SRC_DIR/build/compile_commands.json" ]; then
    cmake -B "$SRC_DIR/build" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
fi

# Run cppcheck with comprehensive checks
CPPCHECK_OUTPUT_FILE="$SRC_DIR/cppcheck_results.xml"
cppcheck \
    --enable=all \
    --inconclusive \
    --error-exitcode=1 \
    --force \
    --inline-suppr \
    --project="$SRC_DIR/build/compile_commands.json" \
    --suppress=missingIncludeSystem \
    --xml \
    --output-file="$CPPCHECK_OUTPUT_FILE"

# If we get here, cppcheck passed
echo "Static analysis complete. Results saved to cppcheck_results.xml"
