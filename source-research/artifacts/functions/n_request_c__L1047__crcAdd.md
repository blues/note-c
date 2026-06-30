# `_crcAdd`

## Status

Finalized. Source-inspected outgoing request CRC inserter.

## Identity

- Kind: function
- Definition: `n_request.c:1047`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `char *_crcAdd(char *json, uint16_t seqno)`
- Visibility: internal to `n_request.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_hooks.c:544:_n_htoa32` finalized
  - `function:n_cjson.c:728:_n_htoa16` finalized
  - `function:n_request.c:1019:_crc32` finalized
- External/system dependencies: `strlen`, `memchr`, `memcpy`
- Data/type/global dependencies: `CRC_FIELD_LENGTH`

## Behavior

Rejects JSON shorter than `{}` or not ending in `}`. Allocates a new buffer of input length plus CRC field length, copies the input without its final brace, inserts either a space for objects with no colon or a comma otherwise, appends `"crc":"SSSS:CCCCCCCC"}` using uppercase hex sequence and CRC32 of the original input bytes including the original closing brace, NUL-terminates, and returns the new buffer. Allocation failure returns NULL.

## Important Boundaries

- Preconditions and assumptions: `json` must be non-null and NUL-terminated.
- Error/null/empty behavior: invalid tail/length or allocation failure returns NULL.
- Ownership and lifetime: caller owns the returned buffer and retains ownership of the input.
- Concurrency/global state: allocator behavior follows note-c hooks.
- Performance shape: linear in JSON length for length, field detection, copy, and CRC.
- Portability/platform constraints: compiled only when CRC code is included outside low-memory builds.

## Direct Call Sites

Exact source search found `_noteTransactionShouldLock` as the production caller in the request-only CRC branch.

## Role In Callers

CRC add failure is nonfatal; the transaction proceeds without response CRC enforcement for that request.

## Tests Observed

`_crcAdd_test.cpp` covers allocation failure, empty string, invalid tail, empty-object formatting, and normal request formatting. `NoteTransaction_test.cpp` covers CRC-add orchestration.

## Graph Links

- Graph node id: `function:n_request.c:1047:_crcAdd`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
