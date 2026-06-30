# `NoteGetLocationMode`

## Status

Finalized. Source-inspected public location-mode read helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1501`
- Declaration/interface: `note.h:1904`
- Public/exported names: `NoteGetLocationMode`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteGetLocationMode(char *modeBuf, int modeBufLen)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: creates a request object through `NoteNewRequest`, receives a response object from `NoteRequestResponse`, and deletes the response with `NoteDeleteResponse`

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_str.c:31:strlcpy` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `macro:note.h:486:NoteDeleteResponse` finalized
- External/system dependencies: Notecard request/response transaction path through `NoteRequestResponse`
- Data/type/global dependencies: transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Clears `modeBuf[0]`, sends a `card.location.mode` request through `NoteRequestResponse(NoteNewRequest(...))`, and returns false when no response is returned. When a response exists, returns false if `NoteResponseError(rsp)` reports an error. On a non-error response, copies `JGetString(rsp, "mode")` into `modeBuf` with `strlcpy`, deletes the response, and returns true.

## Important Boundaries

- Preconditions and assumptions: `modeBuf` must be non-null because the function writes `modeBuf[0]` before any guard. `modeBufLen` is passed directly to `strlcpy`; non-positive lengths are not locally rejected.
- Error/null/empty behavior: no response returns false. Error responses return false after response cleanup. A missing or non-string `"mode"` value is handled by finalized `JGetString` as an empty string, so a non-error response can still return true with an empty copied mode.
- Ownership and lifetime: `NoteRequestResponse` owns the request created by `NoteNewRequest`; the response object is deleted by `NoteDeleteResponse` on every non-null response path.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: uses project-local `strlcpy`; no platform-specific behavior in this wrapper.
- Performance shape: one request allocation/transaction, one response error check, one bounded string copy, and one response delete.
- Security or parsing constraints: truncation is not reported; the function trusts response shape apart from `NoteResponseError`.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1904`, the definition in `n_helpers.c:1501`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that retrieve the Notecard location mode.

## Tests Observed

Focused tests in `test/src/NoteGetLocationMode_test.cpp` fake `NoteNewRequest` and `NoteRequestResponse`. Covered cases: no response returns false, error response returns false, and a response containing `"mode"` returns true. Gaps include a weak success assertion that compares `modeBuf` with itself instead of the expected mode string, plus no coverage for null output buffer, non-positive buffer length, missing mode, or truncation.

## Graph Links

- Graph node id: `function:n_helpers.c:1501:NoteGetLocationMode`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `NoteRequestResponse`, `NoteResponseError`, `strlcpy`, `JGetString`, and `NoteDeleteResponse`.

## Evidence Gaps

No unresolved source gaps for this node.
