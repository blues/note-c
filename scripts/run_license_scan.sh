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

        # Display critical issues
        echo "Critical License Issues:"
        echo "------------------------"
        grep -E "Critical:" fossology_output.txt | sort | uniq || echo "None found"
        echo

        # Display license findings
        echo "License Findings:"
        echo "----------------"
        grep -E "License:" fossology_output.txt | grep -v "Critical:" | sort | uniq || echo "None found"
        echo

        # Display copyright findings
        echo "Copyright Findings:"
        echo "------------------"
        grep -E "Copyright:" fossology_output.txt | sort | uniq || echo "None found"
        echo

        # Count issues by category
        echo " Issue Count by Category: "
        echo "-------------------------"
        for category in "Critical" "License" "Copyright" "Keyword"; do
            count=$(grep -c "${category}:" fossology_output.txt) || true
            printf "%-15s %3d findings\n" "${category}:" "$count"

            # Check if 'category' is 'Critical' and if 'count' is greater than 0
            if [[ "$category" == "Critical" ]] && [ "$count" -gt 0 ]; then
                has_critical_issues=true
            fi
        done
        echo

        # Display status and details
        if $has_critical_issues; then
            echo "Status: FAILED - Critical license issues found"
            echo
            echo "Review and fix critical license issues before proceeding"
        else
            echo "Status: PASSED - No critical license issues found"
            echo
            echo "Note: Review non-critical findings for potential improvements"
        fi
    }

    # Return 1 if critical issues found, 0 otherwise
    if $has_critical_issues; then
        return 1
    else
        return 0
    fi
}

# Run FOSSology scanner and capture output and exit code
docker run --rm \
    --volume "$SRC_DIR:/note-c/" \
    --workdir /note-c/ \
    fossology/fossology:scanner \
    /bin/fossologyscanner nomos ojo copyright keyword repo 2>&1 | tee fossology_output.txt

generate_summary

# Exit with FOSSology's status code
exit $?
