# `NoteRequestResponse`

## Status

Finalized. Source-inspected public consuming request/response wrapper.

## Identity

- Kind: function
- Definition: `n_request.c:220`
- Declaration/interface: `note.h:363`
- Signature/API shape: `J *NoteRequestResponse(J *req)`
- Visibility: public SDK API
- Public/exported names: `NoteRequestResponse`
- Macro aliases/wrappers: none
- Ownership/lifetime role: consumes and deletes the request object; returns caller-owned response object when non-null

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:411:NoteTransaction` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: request/response JSON type `J`

## Behavior

Returns `NULL` immediately when `req` is `NULL`. This supports inline usage such as `NoteRequestResponse(NoteNewRequest("..."))` when allocation fails.

For a non-null request, it calls `NoteTransaction(req)`, stores the returned response pointer, deletes the request with `JDelete(req)`, and returns the response pointer. It does not inspect the response for `"err"` and does not delete the response; callers must use `NoteResponseError` or equivalent and then release the returned response when finished.

## Important Boundaries

- Preconditions and assumptions: caller passes either NULL or an owning `J *` request.
- Error/null/empty behavior: null request returns NULL without calling `NoteTransaction`; null transaction result is returned after the request is deleted.
- Ownership and lifetime: always consumes a non-null request object; transfers response ownership to caller.
- Concurrency/global state: delegates transaction locking and transport behavior to `NoteTransaction`.
- Performance shape: one transaction plus JSON request deletion.
- Portability/platform constraints: all platform I/O remains behind `NoteTransaction` and hook layers.

## Direct Call Sites

Exact source search found production callers in `NoteRequest`, `NoteRequestResponseWithRetry`, and many high-level helpers in `n_helpers.c`, including binary-store length/reset/transmit handshake paths, time/location/status/env/service helpers, sleep/payload retrieval, route sending, voltage/temperature/contact helpers, and debug-sync polling.

Tests and HITL helpers also fake or call this API in focused helper coverage.

## Role In Callers

This is the standard public convenience wrapper for request APIs that need a response body and want the request object freed automatically. Bool-style helper wrappers use `NoteRequest` when they only need success/failure. Higher-level helpers use this function, copy or detach fields from the returned response, and then call `NoteDeleteResponse`/`JDelete`.

## Tests Observed

`NoteRequestResponse_test.cpp` covers null request, null `NoteTransaction` response, valid response, and exactly one `NoteTransaction` call for non-null requests. `NoteRequest_test.cpp` covers the bool wrapper over this API. Numerous focused helper tests fake `NoteRequestResponse` to exercise caller-specific response handling.

## Graph Links

- Graph node id: `function:n_request.c:220:NoteRequestResponse`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
