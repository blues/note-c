# `NotePayloadFindSegment`

## Status

Finalized. Source-inspected segmented payload lookup helper, direct caller, and focused tests.

## Identity

- Kind: function
- Definition: `n_helpers.c:2555`
- Declaration/interface: `note.h:2168`
- Public/exported names: `NotePayloadFindSegment`
- Macro aliases/wrappers: none
- Signature/API shape: `bool NotePayloadFindSegment(NotePayloadDesc *desc, const char segtype[NP_SEGTYPE_LEN], void *pdata, uint32_t *plen)`
- Visibility: public note-c payload helper API
- Ownership/lifetime role: returns a borrowed pointer into `desc->data`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `memcpy`, `memcmp`
- Data/type/global dependencies: `NotePayloadDesc`, `NP_SEGTYPE_LEN`, `NP_SEGLEN_LEN`, and `NP_SEGHDR_LEN`
- Artifact coverage: source, declaration, direct caller, payload constants, and focused tests inspected

## Behavior

First writes `NULL` to `*(uint8_t **)pdata` and zero to `*plen`. It then scans `desc->data` while at least one segment header remains in `desc->length`.

Each segment is encoded as a 4-byte segment type followed by a native `uint32_t` payload length and then that many payload bytes. For each header, it copies the length field with `memcpy`. If the first four bytes match `segtype`, it writes the segment length to `*plen`, writes a borrowed pointer to the segment payload (`p + NP_SEGHDR_LEN`) through `pdata`, and returns `true`. Otherwise it advances by header length plus payload length and continues until no full header remains, then returns `false`.

## Important Boundaries

- Preconditions and assumptions: `desc`, `pdata`, and `plen` must be non-null; `pdata` must point to storage for a `uint8_t *`; `segtype` must reference at least `NP_SEGTYPE_LEN` bytes.
- Error/null/empty behavior: output values are preset to null/zero. If `desc->data` is `NULL`, returns `false`. A zero or too-short `desc->length` returns `false` after initialization.
- Ownership and lifetime: successful `pdata` output is borrowed interior storage owned by `desc`; callers must not free it and must not retain it after descriptor mutation/free.
- Concurrency/global state: no global state.
- Portability/platform constraints: segment length is stored and read as native `uint32_t` byte representation; `memcpy` avoids alignment assumptions but not cross-endian serialization concerns.
- Performance shape: O(number of segment headers scanned plus segment type comparisons).
- Security or parsing constraints: matching segments are returned without verifying that the stored segment length fits within remaining descriptor bytes. Nonmatching oversized segments terminate the scan after pointer advancement.

## Direct Call Sites

Exact production caller found by source search:

- `n_helpers.c:2537` `NotePayloadGetSegment` looks up a segment, checks that the returned segment length equals the caller-requested length, and copies bytes to caller storage.

## Role In Callers

`NotePayloadGetSegment` uses this helper as the raw lookup primitive and adds exact-length copy semantics. Public callers can also use this helper directly when they want a borrowed pointer and returned length.

## Tests Observed

`test/src/NotePayload_test.cpp` covers successful lookup for multiple added segments, lookup failure for an absent segment with null/zero outputs, duplicate segment IDs returning the first added segment, and reallocation scenarios where lookups still find both segments.

## Test Coverage Notes

Focused tests do not cover null `desc`, null output pointers, null `segtype`, truncated/corrupt segment headers, stored lengths that exceed descriptor length, or native-endian portability.

## Graph Links

- Graph node id: `function:n_helpers.c:2555:NotePayloadFindSegment`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
