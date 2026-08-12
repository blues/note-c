#!/bin/bash
#
# Build the JLayout suite in both J layouts, prove they render identically, and
# tabulate the memory difference.
#
# The historical layout is the default; NOTE_C_STORAGE_OPTIMIZATION selects the
# packed one. Because `J` is a single type selected at compile time, the two
# layouts cannot coexist in one binary, so the A/B comparison spans two builds:
#
#   - The output column of every (document, mode) row must be byte-identical.
#     Any difference is a correctness failure, not a tuning result.
#   - The allocation and byte columns are the measurement.
#
# Precision is crossed in because it is not independent of layout. JNUMBER is a
# float under NOTE_C_SINGLE_PRECISION, which shrinks the historical node too --
# on a 32-bit target sizeof(J) is 40 in BOTH layouts there, so the member
# reorder contributes nothing and the entire win comes from inline packing.
# Reporting one set of numbers without saying which precision produced them
# overstates the saving for single-precision targets.
#
# Usage: scripts/run_ab_layout_comparison.sh [--keep] [--single-precision]
#
set -euo pipefail

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR="$SCRIPT_DIR/.."
cd "$ROOT_DIR"

KEEP=0
SINGLE=0
for arg in "$@"; do
    case "$arg" in
        --keep) KEEP=1 ;;
        --single-precision) SINGLE=1 ;;
        *) echo "unknown argument: $arg" >&2; exit 2 ;;
    esac
done

PRECISION_LABEL="double"
PRECISION_OPT=OFF
if [[ $SINGLE -eq 1 ]]; then
    PRECISION_LABEL="single"
    PRECISION_OPT=ON
fi

PACKED_BUILD="$ROOT_DIR/build-ab-packed"
UNPACKED_BUILD="$ROOT_DIR/build-ab-unpacked"
PACKED_REPORT="$ROOT_DIR/build-ab-packed.tsv"
UNPACKED_REPORT="$ROOT_DIR/build-ab-unpacked.tsv"

cleanup() {
    if [[ $KEEP -eq 0 ]]; then
        rm -rf "$PACKED_BUILD" "$UNPACKED_BUILD"
    fi
}
trap cleanup EXIT

build_and_run() {
    local dir="$1" report="$2" optimized="$3" label="$4"
    echo "==> building $label"
    # Use the CMake option, not raw CMAKE_C_FLAGS, so this exercises the same
    # PUBLIC propagation path a real consumer gets.
    cmake -B "$dir" -DNOTE_C_BUILD_TESTS:BOOL=ON \
        -DNOTE_C_STORAGE_OPTIMIZATION:BOOL="$optimized" \
        -DNOTE_C_SINGLE_PRECISION:BOOL="$PRECISION_OPT" > /dev/null
    cmake --build "$dir" --target JLayout_test -- -j > /dev/null
    echo "==> running $label"
    J_LAYOUT_REPORT="$report" "$dir/test/JLayout_test"
}

echo "==> precision: $PRECISION_LABEL"
build_and_run "$PACKED_BUILD"   "$PACKED_REPORT"   ON  "packed (NOTE_C_STORAGE_OPTIMIZATION)"
build_and_run "$UNPACKED_BUILD" "$UNPACKED_REPORT" OFF "historical layout (default)"

echo
echo "==> verifying both layouts render identically"
if diff <(grep -v '^#\|^@' "$PACKED_REPORT"   | cut -f1,2,11) \
        <(grep -v '^#\|^@' "$UNPACKED_REPORT" | cut -f1,2,11) > /dev/null; then
    echo "    OK: every document renders byte-for-byte the same in both layouts"
else
    echo "    FAIL: the layouts disagree on rendered output:"
    diff <(grep -v '^#\|^@' "$PACKED_REPORT"   | cut -f1,2,11) \
         <(grep -v '^#\|^@' "$UNPACKED_REPORT" | cut -f1,2,11) | head -40
    exit 1
fi

echo
J_AB_PRECISION="$PRECISION_LABEL" python3 - "$PACKED_REPORT" "$UNPACKED_REPORT" <<'PYEOF'
import os, sys

def load(path):
    rows, meta = {}, {}
    for line in open(path):
        line = line.rstrip("\n")
        if line.startswith("#"):
            if "sizeof_J=" in line:
                for tok in line.lstrip("# ").split():
                    if "=" in tok:
                        k, v = tok.split("=", 1)
                        meta[k] = v
            continue
        if line.startswith("@"):
            continue
        f = line.split("\t")
        # allocs, hostbytes, nodes, textlen, raw32, dlmalloc, freertos, nano
        rows[(f[0], f[1])] = tuple(int(x) for x in f[2:10])
    return rows, meta

pk, pm = load(sys.argv[1])
uk, um = load(sys.argv[2])

docs = [d for (d, m) in sorted(uk) if m == "compact"]

def col(rows, d, i):
    return rows[(d, "compact")][i]

ALLOCS, HOSTB, NODES, TEXT, RAW32, DL, RTOS, NANO = range(8)

print("=" * 108)
PRECISION = os.environ.get("J_AB_PRECISION", "double")
print(f"J LAYOUT A/B  --  heap held by a parsed document, 32-bit target, {PRECISION} precision")
print("=" * 108)
print("Allocator models (all 32-bit):")
print("  raw       bytes as requested; no allocator overhead at all")
print("  dlmalloc  newlib full malloc: 4B chunk header, 8B align, 16B minimum")
print("  heap_4    FreeRTOS heap_4: 8B BlockLink_t header, 8B align")
print("  nano      newlib nano-malloc: 4B header, 8B align, 8B minimum")
print()
hdr = (f"{'document':<22}{'text':>5}{'node':>5}{'alloc':>6}"
       f"{'raw u':>8}{'raw p':>7}{'':>5}"
       f"{'dlm u':>8}{'dlm p':>7}{'save':>6}"
       f"{'rtos u':>8}{'rtos p':>7}{'save':>6}"
       f"{'nano u':>8}{'nano p':>7}{'save':>6}")
print(hdr)
print("-" * 108)

tot = {k: [0, 0] for k in ("raw", "dl", "rtos", "nano", "alloc")}
ttext = 0
for d in docs:
    ttext += col(uk, d, TEXT)
    vals = {}
    for key, idx in (("raw", RAW32), ("dl", DL), ("rtos", RTOS), ("nano", NANO)):
        u, p = col(uk, d, idx), col(pk, d, idx)
        tot[key][0] += u
        tot[key][1] += p
        vals[key] = (u, p, (100.0 * (u - p) / u) if u else 0.0)
    tot["alloc"][0] += col(uk, d, ALLOCS)
    tot["alloc"][1] += col(pk, d, ALLOCS)
    print(f"{d:<22}{col(uk,d,TEXT):>5}{col(uk,d,NODES):>5}"
          f"{col(uk,d,ALLOCS):>3}/{col(pk,d,ALLOCS):<2}"
          f"{vals['raw'][0]:>8}{vals['raw'][1]:>7}{'':>5}"
          f"{vals['dl'][0]:>8}{vals['dl'][1]:>7}{vals['dl'][2]:>5.0f}%"
          f"{vals['rtos'][0]:>8}{vals['rtos'][1]:>7}{vals['rtos'][2]:>5.0f}%"
          f"{vals['nano'][0]:>8}{vals['nano'][1]:>7}{vals['nano'][2]:>5.0f}%")

print("-" * 108)
def pct(k):
    u, p = tot[k]
    return (100.0 * (u - p) / u) if u else 0.0
print(f"{'TOTAL':<22}{ttext:>5}{'':>5}"
      f"{tot['alloc'][0]:>3}/{tot['alloc'][1]:<2}"
      f"{tot['raw'][0]:>8}{tot['raw'][1]:>7}{'':>5}"
      f"{tot['dl'][0]:>8}{tot['dl'][1]:>7}{pct('dl'):>5.0f}%"
      f"{tot['rtos'][0]:>8}{tot['rtos'][1]:>7}{pct('rtos'):>5.0f}%"
      f"{tot['nano'][0]:>8}{tot['nano'][1]:>7}{pct('nano'):>5.0f}%")
print()
print("=" * 108)
print(f"SUMMARY (32-bit target, {PRECISION} precision)")
print("=" * 108)
SZ = ("48", "40") if PRECISION == "double" else ("40", "40")
print(f"  sizeof(J):        historical {SZ[0]}  ->  optimized {SZ[1]}")
if PRECISION != "double":
    print("                    (identical: a float JNUMBER already removes the")
    print("                     padding, so the whole saving here comes from")
    print("                     inline packing rather than the member reorder)")
au, ap = tot["alloc"]
print(f"  allocations:      {au:>6}  ->  {ap:>6}   ({100.0*(au-ap)/au:.0f}% fewer)")
for label, key in (("raw (no overhead)", "raw"), ("newlib dlmalloc", "dl"),
                   ("FreeRTOS heap_4", "rtos"), ("newlib nano-malloc", "nano")):
    u, p = tot[key]
    print(f"  {label:<18}{u:>6}  ->  {p:>6}   ({100.0*(u-p)/u:.0f}% smaller)"
          f"   [{u/ttext:.1f}x -> {p/ttext:.1f}x the JSON text]")
print()
print("  The raw column is the floor. A real allocator's per-chunk header and")
print("  rounding amplify the win, because the packed layout removes whole")
print("  allocations rather than just shrinking them.")
PYEOF

if [[ $KEEP -eq 0 ]]; then
    rm -f "$PACKED_REPORT" "$UNPACKED_REPORT"
fi
