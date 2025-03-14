#!/bin/bash
set -eo pipefail

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
SRC_DIR="$SCRIPT_DIR/.."

echo "Running License Compliance Analysis..."
echo

# Create a function to generate the summary
generate_summary() {
    {
        # Initialize flag
        has_critical_issues=false

        echo

        # Always generate and display summary regardless of exit code
        echo "=== License Compliance Summary ==="
        echo

        # Display license information
        echo "License Information:"
        echo "-------------------"
        jq -r '.summary.declared_license_expression' scancode_output.json || echo "None found"
        echo

        # Display license clarity score
        echo "License Clarity Score:"
        echo "---------------------"
        jq -r '.summary.license_clarity_score.score' scancode_output.json || echo "Not available"
        echo

        # Display other licenses found
        echo "Other Licenses Found:"
        echo "--------------------"
        jq -r '.summary.other_license_expressions[] | select(.value != null) | "\(.value): \(.count) occurrences"' scancode_output.json || echo "None found"
        echo

        # Display copyright holders
        echo "Copyright Holders:"
        echo "-----------------"
        jq -r '.tallies.holders[] | select(.value != null) | "\(.value): \(.count) occurrences"' scancode_output.json || echo "None found"
        echo

        # Check for non-compliant licenses
        echo "License Compliance Issues:"
        echo "-------------------------"
        # List of non-compliant or problematic licenses
        problematic_licenses=("gpl-2.0" "gpl-3.0" "agpl-3.0" "cc-by-nc" "proprietary")
        
        found_problematic=false
        for license in "${problematic_licenses[@]}"; do
            count=$(jq -r ".summary.other_license_expressions[] | select(.value == \"$license\") | .count" scancode_output.json 2>/dev/null || echo "0")
            if [ "$count" != "0" ] && [ "$count" != "" ]; then
                echo "WARNING: Found $count occurrences of $license license, which may have compliance implications."
                found_problematic=true
                has_critical_issues=true
            fi
        done
        
        if ! $found_problematic; then
            echo "No problematic licenses found."
        fi
        echo

        # Display status and details
        if $has_critical_issues; then
            echo "Status: FAILED - License compliance issues found"
            echo
            echo "Review and fix license compliance issues before proceeding"
        else
            echo "Status: PASSED - No license compliance issues found"
            echo
            echo "Note: Review license information for potential improvements"
        fi
    }

    # Return 1 if critical issues found, 0 otherwise
    if $has_critical_issues; then
        return 1
    else
        return 0
    fi
}

# Run scancode and capture output and exit code
scancode \
    --license \
    --copyright \
    --classify \
    --summary \
    --license-clarity-score \
    --tallies \
    --json-pp scancode_output.json \
    --timeout 120 \
    --processes 2 \
    . 2>&1 | tee scancode_console_output.txt

generate_summary

# Exit with scancode's status code
exit $?
