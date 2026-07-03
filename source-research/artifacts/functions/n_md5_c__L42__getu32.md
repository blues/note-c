# `getu32`

## Status

Finalized. Source-inspected leaf helper for MD5 little-endian word loading.

## Identity

- Kind: function
- Definition: `n_md5.c:42`
- Prototype: `static unsigned long getu32 (const unsigned char *addr);` at `n_md5.c:34`
- Visibility: file-local static helper
- Public/exported names: none
- Macro aliases/wrappers: none
- Signature/API shape: `static unsigned long getu32(const unsigned char *addr)`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: primitive byte indexing, shifts, and bitwise OR
- Data/type/global dependencies: none
- Artifact coverage: source implementation, prototype, graph node, and direct caller search inspected

## Behavior

Reads four bytes from `addr[0..3]` and returns a little-endian 32-bit word widened to `unsigned long`. The code builds the word by shifting `addr[3]`, OR-ing in `addr[2]`, `addr[1]`, and `addr[0]`, so it is independent of host byte order and does not require `unsigned long` to be exactly 32 bits.

## Important Boundaries

- Preconditions and assumptions: caller must provide at least four readable bytes; there is no null or bounds check.
- Error/null/empty behavior: none; invalid pointers are undefined C behavior.
- Ownership and lifetime: borrows the input pointer and allocates nothing.
- Concurrency/global state: no static mutable or global state.
- Portability/platform constraints: explicitly byte-oriented little-endian decode.
- Performance shape: O(1).

## Direct Call Sites

Exact source search `rg -n "\bgetu32\b" n_md5.c note.h` found:

- `n_md5.c:34` static prototype.
- `n_md5.c:42` definition.
- `n_md5.c:208` called by `NoteMD5Transform` while unpacking 16 input words.

## Role In Callers

`NoteMD5Transform` uses `getu32(inraw + 4 * i)` to convert each 4-byte chunk of a 64-byte MD5 block into the working `in[16]` array before running the MD5 rounds.

## Tests Observed

None. This static helper has no direct focused production-symbol test inspected.

## Graph Links

- Graph node id: `function:n_md5.c:42:getu32`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming edge: `function:n_md5.c:201:NoteMD5Transform` -> `function:n_md5.c:42:getu32`

## Evidence Gaps

No unresolved source gaps for this node.
