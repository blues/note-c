# `NotePayloadGetSegment`

## Status

Finalized. Source-inspected exact-length segmented payload copy helper, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_helpers.c:2533`
- Declaration/interface: `note.h:2179`
- Public/exported names: `NotePayloadGetSegment`
- Macro aliases/wrappers: none
- Signature/API shape: `bool NotePayloadGetSegment(NotePayloadDesc *desc, const char segtype[NP_SEGTYPE_LEN], void *pdata, uint32_t len)`
- Visibility: public note-c payload helper API
- Ownership/lifetime role: copies from descriptor-owned payload storage into caller-provided buffer

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:2555:NotePayloadFindSegment` finalized
- External/system dependencies: `memcpy`
- Data/type/global dependencies: `NotePayloadDesc` and payload segment constants through the lookup helper
- Artifact coverage: source, declaration, direct caller search, and focused tests inspected

## Behavior

Calls `NotePayloadFindSegment(desc, segtype, &data, &datalen)`. If the lookup succeeds and the returned segment length exactly equals the caller-provided `len`, copies `len` bytes from the borrowed segment pointer into `pdata` with `memcpy` and returns `true`. Otherwise returns `false` and does not copy.

## Important Boundaries

- Preconditions and assumptions: caller-provided `pdata` must be writable for `len` bytes when the segment is found with matching length. Other pointer preconditions are inherited from `NotePayloadFindSegment`.
- Error/null/empty behavior: lookup failure and length mismatch return `false`.
- Ownership and lifetime: no allocation; successful calls copy into caller storage and do not transfer ownership of descriptor storage.
- Concurrency/global state: no global state.
- Portability/platform constraints: inherits segment layout constraints from `NotePayloadFindSegment`.
- Performance shape: O(segment scan plus `len` byte copy on success).
- Security or parsing constraints: exact-length check prevents partial copies but trusts the length reported by `NotePayloadFindSegment`.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This is the public copy-oriented segment lookup API.

## Tests Observed

`test/src/NotePayload_test.cpp` covers successful copy from a segment and failure when the requested length does not match the stored segment length.

## Test Coverage Notes

Focused tests do not cover absent segment through this wrapper, null pointers, zero-length copy, or corrupt segment lengths.

## Graph Links

- Graph node id: `function:n_helpers.c:2533:NotePayloadGetSegment`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
