#!/usr/bin/env bash
#
# Update the note-c C library to the latest release or a specified version.
#
# Usage:
#   ./update_note_c.sh [dest_dir] [version]
#
# Arguments:
#   dest_dir  Directory to install note-c files into (default: current directory)
#   version   Release version tag, e.g. v2.5.5 (default: latest)
#
# Examples:
#   ./update_note_c.sh                    # Latest release into current dir
#   ./update_note_c.sh ./lib/note-c       # Latest release into ./lib/note-c
#   ./update_note_c.sh . v2.5.4           # Specific version into current dir

set -euo pipefail

REPO="blues/note-c"
DEST="${1:-.}"
VERSION="${2:-}"

if ! command -v curl &>/dev/null; then
    echo "Error: curl is required but not found." >&2
    exit 1
fi

# Resolve the version tag
if [ -z "${VERSION}" ]; then
    VERSION=$(curl -sfL \
        -H "Accept: application/vnd.github+json" \
        "https://api.github.com/repos/${REPO}/releases/latest" \
        | grep '"tag_name"' | cut -d'"' -f4)
    if [ -z "${VERSION}" ]; then
        echo "Error: failed to determine latest release version." >&2
        exit 1
    fi
    echo "Latest release: ${VERSION}"
else
    echo "Requested release: ${VERSION}"
fi

ARCHIVE="note-c-${VERSION}.tar.gz"
URL="https://github.com/${REPO}/releases/download/${VERSION}/${ARCHIVE}"

# Download the archive
TMPDIR=$(mktemp -d)
trap 'rm -rf "${TMPDIR}"' EXIT

echo "Downloading ${URL}..."
if ! curl -fL -o "${TMPDIR}/${ARCHIVE}" "${URL}"; then
    echo "Error: download failed. Check that version '${VERSION}' exists and has a release archive." >&2
    exit 1
fi

# Extract into destination
mkdir -p "${DEST}"
tar xzf "${TMPDIR}/${ARCHIVE}" -C "${DEST}"

echo "note-c ${VERSION} installed to ${DEST}"
