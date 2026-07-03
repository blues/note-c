# `n_htoa8`

## Status

Finalized. Source-inspected byte-to-hex helper.

## Identity

- Kind: function
- Definition: `n_md5.c:57`
- Forward declaration: `n_md5.c:32`
- Signature/API shape: `void n_htoa8(unsigned char n, unsigned char *p)`
- Visibility: implementation-local helper in `n_md5.c`; not declared in `note.h`
- Public/exported names: none intended, despite non-static definition
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: primitive shifts, masks, comparisons, and byte stores
- Data/type/global dependencies: writes caller-provided output pointer; no global state
- Artifact coverage: implementation, forward declaration, callers, and graph node inspected

## Behavior

Converts one byte to two lowercase hexadecimal characters. It extracts the high nibble with `(n >> 4) & 0xf`, writes either `'a' + nibble - 10` or `'0' + nibble`, repeats the same conversion for the low nibble, then writes a trailing NUL byte.

## Important Boundaries

- Preconditions and assumptions: caller must provide at least three writable bytes at `p`; no null or bounds checks.
- Error/null/empty behavior: none; invalid pointers are undefined C behavior.
- Ownership and lifetime: mutates caller-provided buffer and allocates nothing.
- Concurrency/global state: no global state.
- Portability/platform constraints: ASCII digit/letter assumptions match the SDK's C string usage.
- Performance shape: O(1).

## Direct Call Sites

Exact source search `rg -n "\bn_htoa8\b" n_md5.c` found:

- `n_md5.c:32` forward declaration.
- `n_md5.c:57` definition.
- `n_md5.c:307` called by `NoteMD5HashString`.
- `n_md5.c:318` called by `NoteMD5HashToString`.

## Role In Callers

`NoteMD5HashString` and `NoteMD5HashToString` call this helper once for each of the 16 MD5 digest bytes, writing two hex characters per byte.

## Tests Observed

None. This helper has no direct focused production-symbol test inspected.

## Graph Links

- Graph node id: `function:n_md5.c:57:n_htoa8`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming edges from `NoteMD5HashString` and `NoteMD5HashToString`.

## Evidence Gaps

No unresolved source gaps for this node.
