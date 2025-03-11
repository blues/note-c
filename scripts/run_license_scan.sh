#!/bin/bash
set -eo pipefail

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
SRC_DIR="$SCRIPT_DIR/.."

echo "Running License Compliance Analysis..."
echo

# Install FOSSology scanners if not already installed
if ! command -v nomossa &> /dev/null || ! command -v copyright &> /dev/null; then
    echo "Installing FOSSology scanners..."
    apt-get update -qq
    apt-get install -y --no-install-recommends wget ca-certificates
    
    # Create a temporary directory for FOSSology tools
    mkdir -p /tmp/fossology
    cd /tmp/fossology
    
    # Download and extract FOSSology CLI tools
    wget -q https://github.com/fossology/fossology/releases/download/4.4.0/fossology-4.4.0-1_amd64.deb
    dpkg -x fossology-4.4.0-1_amd64.deb .
    
    # Copy the scanner binaries to a location in PATH
    cp -f usr/lib/fossology/agents/nomossa /usr/local/bin/
    cp -f usr/lib/fossology/agents/ojo /usr/local/bin/
    cp -f usr/lib/fossology/agents/copyright /usr/local/bin/
    
    # Clean up
    cd -
    rm -rf /tmp/fossology
    
    echo "FOSSology scanners installed successfully."
fi

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

# Create output file
touch fossology_output.txt

# Run license scanners directly
echo "Running Nomos license scanner..."
find "$SRC_DIR" -type f -not -path "*/\.*" -not -path "*/build/*" | while read -r file; do
    if [ -f "$file" ]; then
        # Run nomos scanner
        if command -v nomossa &> /dev/null; then
            result=$(nomossa "$file" 2>/dev/null || echo "No license found")
            if [ "$result" != "No license found" ]; then
                echo "License: $result in $file" >> fossology_output.txt
                
                # Check for non-MIT licenses (example of a critical issue)
                if [[ "$result" != *"MIT"* ]] && [[ "$result" != *"SPDX"* ]]; then
                    echo "Critical: Non-MIT license found: $result in $file" >> fossology_output.txt
                fi
            fi
        fi
        
        # Run copyright scanner
        if command -v copyright &> /dev/null; then
            copyright_result=$(copyright "$file" 2>/dev/null || echo "")
            if [ -n "$copyright_result" ]; then
                echo "Copyright: $copyright_result in $file" >> fossology_output.txt
            fi
        fi
    fi
done

# Run keyword scanner (simple implementation)
echo "Running keyword scanner..."
grep -r --include="*.c" --include="*.h" --include="*.cpp" --include="*.hpp" -l "GPL\|GNU" "$SRC_DIR" | while read -r file; do
    echo "Keyword: Potential GPL reference in $file" >> fossology_output.txt
    echo "Critical: Potential GPL license reference in $file" >> fossology_output.txt
done

generate_summary

# Exit with summary's status code
exit $?
