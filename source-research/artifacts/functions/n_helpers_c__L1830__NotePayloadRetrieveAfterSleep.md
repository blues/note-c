# `NotePayloadRetrieveAfterSleep`

## Status

Finalized. Source-inspected public sleep-payload retrieval helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1830`
- Declaration/interface: `note.h:2132`
- Signature/API shape: `bool NotePayloadRetrieveAfterSleep(NotePayloadDesc *desc)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NotePayloadRetrieveAfterSleep`
- Macro aliases/wrappers: none
- Ownership/lifetime role: initializes/fills caller descriptor with allocated decoded payload buffer

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:1950:JAddBoolToObject` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `macro:note.h:486:NoteDeleteResponse` finalized
  - `function:n_cjson_helpers.c:122:JGetInt` finalized
  - `function:n_helpers.c:791:_setTime` finalized in this slice
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `function:n_b64.c:106:JB64DecodeLen` finalized
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_b64.c:121:JB64Decode` finalized
  - `macro:note.h:1006:NOTE_C_LOG_INFO` finalized
- External/system dependencies: `memset`
- Data/type/global dependencies: request/response JSON, time cache through `_setTime`, allocator hook through `_Malloc`
- Artifact coverage: generated artifact exists

## Behavior

Zeroes `*desc` when `desc` is non-null, sends `card.attn` with `"start": true`, and fails on request allocation, null response, or response error. A nonzero response `"time"` refreshes the global time cache with `_setTime`.

If `desc == NULL`, a successful no-error response is enough and the function deletes the response and returns true. Otherwise it requires a nonempty `"payload"` string, allocates `JB64DecodeLen(payload)` bytes, decodes with `JB64Decode`, stores `data`, `alloc`, and `length` in the descriptor, logs success, deletes the response, and returns true.

## Important Boundaries

- Preconditions and assumptions: caller frees a successful descriptor with `NotePayloadFree`; malformed Base64 is not separately validated by this helper.
- Error/null/empty behavior: descriptor is zeroed before failures when non-null; missing/empty payload is failure only when a descriptor was requested; allocation failure deletes the response and returns false.
- Ownership and lifetime: on success with `desc`, descriptor owns the allocated decoded buffer; response strings are borrowed only before response deletion.
- Concurrency/global state: may update global time cache; allocation uses hook-backed `_Malloc`.
- Portability/platform constraints: Base64 decoding and allocation remain in portable helpers/hooks.
- Performance shape: one request transaction plus O(payload length) Base64 sizing/decoding.
- Security or parsing constraints: allocates based on `JB64DecodeLen` and trusts `JB64Decode` output length; no checksum or payload schema validation.

## Direct Call Sites

Exact source search found direct production caller:

- `n_helpers.c:1808` `NoteWake` retrieves and length-checks preserved state.

## Role In Callers

This is the public descriptor-level sleep payload restore API; `NoteWake` is the fixed-length convenience wrapper.

## Tests Observed

Focused `test/src/NotePayloadRetrieveAfterSleep_test.cpp` covers no-payload success, request allocation failure, null response, response error, missing payload, allocation failure, and successful Base64 decode with a `"time"` field.

## Graph Links

- Graph node id: `function:n_helpers.c:1830:NotePayloadRetrieveAfterSleep`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddBoolToObject`, `NoteRequestResponse`, `NoteResponseError`, `NoteDeleteResponse`, `JGetInt`, `_setTime`, `JGetString`, `JB64DecodeLen`, `_Malloc`, `JB64Decode`, and `NOTE_C_LOG_INFO`.

## Evidence Gaps

No unresolved source gaps for this node.
