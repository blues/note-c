# `NotePayloadAddSegment`

## Status

Finalized. Source-inspected public segmented-payload append helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:2473`
- Declaration/interface: `note.h:2157`
- Public/exported names: `NotePayloadAddSegment`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NotePayloadAddSegment(NotePayloadDesc *desc, const char segtype[NP_SEGTYPE_LEN], void *data, uint32_t len)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: mutates and may replace the buffer owned by the caller-provided `NotePayloadDesc`

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:241:_Malloc` finalized
  - `macro:n_lib.h:242:_Free` finalized
- External/system dependencies: libc `memcpy`
- Data/type/global dependencies: `NotePayloadDesc`; payload constants `NP_SEGTYPE_LEN`, `NP_SEGLEN_LEN`, and `NP_SEGHDR_LEN`
- Artifact coverage: source-inspected artifact

## Behavior

Computes segment storage size as `len + NP_SEGHDR_LEN`, starts with a 512-byte allocation target, and raises that target by the segment size when the segment is larger than the default block. If `desc->data` is null, it allocates a new buffer, writes the 4-byte segment type, writes the `uint32_t` segment length, copies `len` payload bytes, and records `data`, `alloc`, and `length` in the descriptor.

If `desc->data` already exists but the remaining capacity is smaller than the new segment, it allocates `desc->alloc + alloc`, copies the old descriptor contents, appends the new segment header and payload, frees the old descriptor buffer, then points the descriptor at the new buffer and extends `alloc` and `length`. If enough capacity remains, it appends the same segment header and payload in place. Allocation failure returns `false`; otherwise the helper returns `true`.

## Important Boundaries

- Preconditions and assumptions: `desc` and `segtype` must be non-null. `segtype` is read for exactly four bytes. `data` must be usable as a `memcpy` source for `len` bytes.
- Error/null/empty behavior: allocation failure returns `false` and leaves the descriptor unchanged in both allocation paths. Null pointers are not guarded before dereference/copy. Zero-length data still writes an 8-byte segment header.
- Ownership and lifetime: allocated storage comes from `_Malloc`/`NoteMalloc`; replaced storage is released through `_Free`/`NoteFree`. A descriptor initialized with external storage can be freed by this helper during growth, so that storage must match the SDK free hook when growth is possible.
- Concurrency/global state: no direct locking; allocator behavior depends on global SDK allocation hooks through `_Malloc` and `_Free`.
- Portability/platform constraints: stores the segment length by copying the in-memory `uint32_t` representation, so the payload segment header follows the platform endianness and width used by this SDK build.
- Performance shape: appends are O(segment length) when capacity remains and O(existing length + segment length) when reallocation is needed. The first allocation reserves at least 512 bytes.
- Security or parsing constraints: no overflow checks for `len + NP_SEGHDR_LEN`, `alloc + hlen`, `desc->alloc - desc->length`, or `desc->alloc + alloc`; malformed descriptor state can make capacity checks unreliable.

## Direct Call Sites

Exact source search found the public declaration in `note.h:2157`, the definition in `n_helpers.c:2473`, and focused tests in `test/src/NotePayload_test.cpp`. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that build an in-memory segmented payload descriptor.

## Tests Observed

Focused tests in `test/src/NotePayload_test.cpp` call `NotePayloadAddSegment` and then use payload retrieval helpers. Covered cases include first allocation and append, finding two segments, missing segment lookup, duplicate segment IDs returning the first segment, and forced reallocation while preserving prior data. Gaps include allocator failure, null `desc`, null `segtype`, null `data`, zero-length payloads, integer overflow, malformed descriptor capacity/length, and external-buffer ownership via `NotePayloadSet`. The test section intended to check an incorrect `NotePayloadGetSegment` length has a precedence issue and does not actually assert the failure path.

## Graph Links

- Graph node id: `function:n_helpers.c:2473:NotePayloadAddSegment`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_Malloc` and `_Free`; libc `memcpy` is an external dependency.

## Evidence Gaps

No unresolved source gaps for this node.
