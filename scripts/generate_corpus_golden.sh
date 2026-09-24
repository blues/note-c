#!/bin/bash
#
# Record what the HISTORICAL implementation renders for every corpus document,
# and check the result in as test/data/.
#
# WHY THIS EXISTS. The layout suite builds the library twice and diffs the two
# runs. That proves the two layouts agree with EACH OTHER -- it cannot prove
# either agrees with the note-c that shipped, because both builds run the same
# rewritten parser, printer and setters. Any regression they share is invisible
# to a comparison between them.
#
# So the reference has to come from outside the working tree. This script
# extracts a pristine `git archive HEAD` (or any revision named on the command
# line), compiles the corpus against THAT source, and records its output. The
# layout suite then compares against the recorded file, which is a claim about
# the released behavior rather than about the rewrite's self-consistency.
#
# The generator deliberately links nothing from the working tree except
# test/include/j_corpus.hpp, which is why that header is kept free of Catch2 and
# of anything newer than the public API.
#
# Usage: scripts/generate_corpus_golden.sh [revision]     (default: HEAD)
#
set -euo pipefail

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR="$SCRIPT_DIR/.."
cd "$ROOT_DIR"

REV="${1:-HEAD}"
OUT_DIR="$ROOT_DIR/test/data"
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT

REV_SHA=$(git rev-parse --short "$REV")
echo "==> extracting $REV ($REV_SHA)"
git archive "$REV" | tar -x -C "$WORK"

# The corpus header comes from the WORKING TREE, the library from the archive.
cp test/include/j_corpus.hpp "$WORK/"

cat > "$WORK/gen.cpp" <<'EOF'
// Renders every (document, mode) pair and writes one TSV row each.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "j_corpus.hpp"

using namespace jlayout;

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: gen <output>\n");
        return 2;
    }
    FILE *f = fopen(argv[1], "w");
    if (f == NULL) {
        return 2;
    }

    NoteSetFn(malloc, free, NULL, NULL);

    fprintf(f, "# Rendered by the historical note-c implementation.\n");
    fprintf(f, "# Regenerate with scripts/generate_corpus_golden.sh -- do not hand-edit.\n");
    fprintf(f, "# revision\t%s\n", getenv("GOLDEN_REV") ? getenv("GOLDEN_REV") : "?");
    fprintf(f, "# precision\t%s\n",
#ifdef NOTE_C_SINGLE_PRECISION
            "single"
#else
            "double"
#endif
           );
    fprintf(f, "#doc\tmode\toutput\n");

    const std::vector<Doc> &docs = corpus();
    const std::vector<Mode> &ms = modes();
    for (size_t d = 0; d < docs.size(); ++d) {
        J *parsed = JParse(docs[d].json);
        if (parsed == NULL) {
            fprintf(stderr, "FATAL: %s did not parse\n", docs[d].name);
            return 1;
        }
        for (size_t m = 0; m < ms.size(); ++m) {
            char *out = (char *)_print(parsed, ms[m].format, ms[m].omitempty);
            if (out == NULL) {
                fprintf(stderr, "FATAL: %s/%s did not print\n", docs[d].name, ms[m].name);
                return 1;
            }
            // Escape so one row stays one line.
            fprintf(f, "%s\t%s\t", docs[d].name, ms[m].name);
            for (const char *p = out; *p; ++p) {
                if (*p == '\n') {
                    fputs("\\n", f);
                } else if (*p == '\t') {
                    fputs("\\t", f);
                } else if (*p == '\\') {
                    fputs("\\\\", f);
                } else {
                    fputc(*p, f);
                }
            }
            fputc('\n', f);
            JFree(out);
        }
        JDelete(parsed);
    }

    fclose(f);
    return 0;
}
EOF

# n_str.c only compiles when the platform lacks strlcpy/strlcat, and we define
# both below, exactly as CMakeLists.txt decides.
SRC=$(cd "$WORK" && ls n_*.c | grep -v '^n_str\.c$' | tr '\n' ' ')
mkdir -p "$OUT_DIR"

build_one() {
    local label="$1" out="$2" extra="${3:-}"
    echo "==> building generator against $REV_SHA ($label)"
    # The library is compiled as C and the generator as C++, exactly as the
    # CMake test build does. NOTE_C_TEST empties NOTE_C_STATIC so the generator
    # can reach _print, which is the same entry point the layout suite renders
    # through.
    (cd "$WORK" \
        && rm -f ./*.o \
        && cc -std=c99 -O1 -I. -DNOTE_C_TEST -DHAVE_STRLCPY -DHAVE_STRLCAT $extra \
              -c $SRC \
        && c++ -std=c++14 -O1 -I. -DNOTE_C_TEST -DHAVE_STRLCPY -DHAVE_STRLCAT $extra \
               gen.cpp ./*.o -o gen) > "$WORK/build.log" 2>&1 || {
        echo "generator build failed:"; grep -m10 "error:" "$WORK/build.log"; exit 1;
    }
    echo "==> recording $label -> $out"
    GOLDEN_REV="$REV_SHA" "$WORK/gen" "$out"
    echo "    $(grep -vc '^#' "$out") rows"
}

build_one "default precision" "$OUT_DIR/corpus_golden.tsv"
build_one "single precision"  "$OUT_DIR/corpus_golden_single.tsv" "-DNOTE_C_SINGLE_PRECISION"

echo
echo "Goldens written to test/data/. Review the diff before committing:"
echo "    git diff --stat test/data/"
