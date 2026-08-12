#!/usr/bin/env bash
#
# Update the note-c C library to the latest release or a specified version.
#
# Usage:
#   ./update_note_c.sh [options] [dest_dir] [version]
#
# Arguments:
#   dest_dir  Directory to install note-c files into (default: current directory)
#   version   Release version tag, e.g. v2.5.5 (default: latest)
#
# Options:
#   --clean [dir]  Remove all non-library files from dir (default: current
#                  directory). This is useful after cloning the repo to strip
#                  it down to just the library. The script removes itself.
#
# Examples:
#   ./update_note_c.sh                    # Latest release into current dir
#   ./update_note_c.sh ./lib/note-c       # Latest release into ./lib/note-c
#   ./update_note_c.sh . v2.5.4           # Specific version into current dir
#   ./update_note_c.sh --clean            # Clean current dir to library only
#   ./update_note_c.sh --clean ./note-c   # Clean a specific directory

set -euo pipefail

# Library files to keep during --clean
LIBRARY_FILES=(
    LICENSE
    note.h
    n_lib.h
    n_cjson.h
    n_atof.c
    n_b64.c
    n_cjson.c
    n_cjson_helpers.c
    n_cobs.c
    n_const.c
    n_ftoa.c
    n_helpers.c
    n_hooks.c
    n_i2c.c
    n_md5.c
    n_printf.c
    n_request.c
    n_serial.c
    n_str.c
    n_ua.c
)

clean() {
    local dir="${1:-.}"
    dir=$(cd "${dir}" && pwd)

    # Remove everything in the directory that isn't a library file
    for entry in "${dir}"/* "${dir}"/.*; do
        name=$(basename "${entry}")
        [ "${name}" = "." ] || [ "${name}" = ".." ] && continue

        local is_library=false
        for f in "${LIBRARY_FILES[@]}"; do
            if [ "${name}" = "${f}" ]; then
                is_library=true
                break
            fi
        done

        if [ "${is_library}" = false ]; then
            rm -rf "${entry}"
        fi
    done

    echo "Cleaned ${dir} to library files only."
}

if [ "${1:-}" = "--clean" ]; then
    clean "${2:-.}"
    exit 0
fi

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
