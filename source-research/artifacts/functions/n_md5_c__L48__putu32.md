# `putu32`

## Status

Finalized. Source-inspected leaf helper for MD5 little-endian word storage.

## Identity

- Kind: function
- Definition: `n_md5.c:48`
- Prototype: `static void putu32 (unsigned long data, unsigned char *addr);` at `n_md5.c:33`
- Visibility: file-local static helper
- Public/exported names: none
- Macro aliases/wrappers: none
- Signature/API shape: `static void putu32(unsigned long data, unsigned char *addr)`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: primitive byte stores and right shifts
- Data/type/global dependencies: none
- Artifact coverage: source implementation, prototype, graph node, and direct caller search inspected

## Behavior

Stores the low 32 bits of `data` to `addr[0..3]` in little-endian byte order. Each byte is assigned by truncating `data`, `data >> 8`, `data >> 16`, and `data >> 24` to `unsigned char`.

## Important Boundaries

- Preconditions and assumptions: caller must provide at least four writable bytes; there is no null or bounds check.
- Error/null/empty behavior: none; invalid pointers are undefined C behavior.
- Ownership and lifetime: mutates caller-provided output memory and allocates nothing.
- Concurrency/global state: no static mutable or global state.
- Portability/platform constraints: explicitly byte-oriented little-endian encode.
- Performance shape: O(1).

## Direct Call Sites

Exact source search `rg -n "\bputu32\b" n_md5.c note.h` found:

- `n_md5.c:33` static prototype.
- `n_md5.c:48` definition.
- `n_md5.c:173`, `n_md5.c:174` write the bit counts into the MD5 final block.
- `n_md5.c:177`-`n_md5.c:180` write the four state words into the caller-provided digest.

## Role In Callers

`NoteMD5Final` uses `putu32` for both padding metadata and final digest serialization.

## Tests Observed

None. This static helper has no direct focused production-symbol test inspected.

## Graph Links

- Graph node id: `function:n_md5.c:48:putu32`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming edge: `function:n_md5.c:143:NoteMD5Final` -> `function:n_md5.c:48:putu32`

## Evidence Gaps

No unresolved source gaps for this node.
