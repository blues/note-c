#!/bin/bash
set -eo pipefail

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
SRC_DIR="$SCRIPT_DIR/.."

# Ensure build directory exists and compile_commands.json is generated
if [ ! -d "$SRC_DIR/build" ] || [ ! -f "$SRC_DIR/build/compile_commands.json" ]; then
    cmake -B "$SRC_DIR/build" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
fi

echo "=== Running Static Analysis ==="
echo

# Create a function to generate the summary
generate_summary() {
    {
        echo "=== Static Analysis Summary ==="
        echo
        
        echo "Critical Issues (Errors & Warnings):"
        echo "-----------------------------------"
        grep -E "error:|warning:" cppcheck_output.txt | grep -v "Checking " | grep -v "nofile:0:" | \
            sort | uniq | awk -F': ' '{printf "%-40s %s\n", $1, $4}' || echo "None found"
        echo
        
        echo "Performance & Portability Issues:"
        echo "--------------------------------"
        grep -E "performance:|portability:" cppcheck_output.txt | grep -v "Checking " | \
            sort | uniq | awk -F': ' '{printf "%-40s %s\n", $1, $4}' || echo "None found"
        echo
        
        echo "Issue Count by Severity:"
        echo "------------------------"
        for sev in error warning performance portability style information; do
            count=$(grep -c "${sev}:" cppcheck_output.txt || echo 0)
            printf "%-12s %d issues\n" "${sev^^}:" "$count"
        done
        echo
        
        if [ $CPPCHECK_EXIT_CODE -ne 0 ]; then
            echo "Status: FAILED - Critical issues found"
            echo
            echo "Critical Issues Details:"
            echo "------------------------"
            grep -E "error:|warning:" cppcheck_output.txt | grep -v "Checking " | grep -v "nofile:0:" | \
                sort | uniq || echo "None found"
        else
            echo "Status: PASSED - No critical issues found"
            echo "Note: Review non-critical issues for potential improvements"
        fi
    } | tee summary.txt
}

# Run cppcheck and capture all output
# Run cppcheck and capture output
cppcheck \
    --enable=all \
    --check-level=exhaustive \
    --inconclusive \
    --std=c11 \
    --force \
    --inline-suppr \
    --suppress=missingIncludeSystem \
    --suppress=nullPointerRedundantCheck:*/n_cjson.c \
    --suppress=ctunullpointer:*/n_cjson.c \
    --suppress=unusedFunction \
    --suppress=unmatchedSuppression \
    --suppress=style \
    --suppress=information \
    --suppress=syntaxError:test/* \
    --suppress=unknownMacro:test/* \
    -I test/include \
    --template="{file}:{line}: {severity}: {id}: {message}" \
    --max-configs=32 \
    --check-library \
    --debug-warnings \
    --error-exitcode=1 \
    . > >(tee cppcheck_output.txt) 2>&1
CPPCHECK_EXIT_CODE=${PIPESTATUS[0]}

# Generate summary
echo "=== Static Analysis Summary ===" > summary.txt
echo >> summary.txt

echo "Critical Issues (Errors & Warnings):" >> summary.txt
echo "-----------------------------------" >> summary.txt
grep -E "error:|warning:" cppcheck_output.txt | grep -v "Checking " | grep -v "nofile:0:" | \
    sort | uniq | awk -F': ' '{printf "%-40s %s\n", $1, $4}' >> summary.txt || echo "None found" >> summary.txt
echo >> summary.txt

echo "Performance & Portability Issues:" >> summary.txt
echo "--------------------------------" >> summary.txt
grep -E "performance:|portability:" cppcheck_output.txt | grep -v "Checking " | \
    sort | uniq | awk -F': ' '{printf "%-40s %s\n", $1, $4}' >> summary.txt || echo "None found" >> summary.txt
echo >> summary.txt

echo "Issue Count by Severity:" >> summary.txt
echo "------------------------" >> summary.txt
for sev in error warning performance portability style information; do
    count=$(grep -c "${sev}:" cppcheck_output.txt || echo 0)
    printf "%-12s %d issues\n" "${sev^^}:" "$count" >> summary.txt
done
echo >> summary.txt

if [ $CPPCHECK_EXIT_CODE -ne 0 ]; then
    echo "Status: FAILED - Critical issues found" >> summary.txt
    echo >> summary.txt
    echo "Critical Issues Details:" >> summary.txt
    echo "------------------------" >> summary.txt
    grep -E "error:|warning:" cppcheck_output.txt | grep -v "Checking " | grep -v "nofile:0:" | \
        sort | uniq >> summary.txt || echo "None found" >> summary.txt
else
    echo "Status: PASSED - No critical issues found" >> summary.txt
    echo "Note: Review non-critical issues for potential improvements" >> summary.txt
fi

# Display summary
cat summary.txt

# Exit with cppcheck's exit code
exit $CPPCHECK_EXIT_CODE
