# `NoteGetNetStatus`

## Status

Finalized. Source-inspected public `hub.status` helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1368`
- Declaration/interface: `note.h:1859`
- Public/exported names: `NoteGetNetStatus`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteGetNetStatus(char *statusBuf, int statusBufLen)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: writes into caller-owned buffer and owns/deletes the response object it receives

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `macro:note.h:479:NoteResponseError` expands through finalized `JIsNullString`
  - `function:n_str.c:31:strlcpy` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `macro:note.h:486:NoteDeleteResponse` expands to finalized `JDelete`
- External/system dependencies: Notecard transaction path through `NoteRequestResponse`
- Data/type/global dependencies: transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Clears `statusBuf[0]`, sends a `hub.status` request with `NoteRequestResponse(NoteNewRequest("hub.status"))`, and returns false when no response is returned. For a response, treats `!NoteResponseError(rsp)` as success; on success, copies `JGetString(rsp, "status")` into `statusBuf` with `strlcpy(statusBuf, ..., statusBufLen)`. Always deletes a non-null response before returning.

## Important Boundaries

- Preconditions and assumptions: `statusBuf` must be non-null and writable; `statusBufLen` should describe its capacity.
- Error/null/empty behavior: request allocation failure flows through `NoteRequestResponse(NULL)` to false after clearing the buffer. Error responses return false. Missing or non-string `"status"` can still return true and copy an empty string.
- Ownership and lifetime: caller owns `statusBuf`; `NoteRequestResponse` owns/frees the request, and this helper deletes the returned response.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: uses the bundled or platform `strlcpy`.
- Performance shape: one request/response transaction plus bounded buffer copy.
- Security or parsing constraints: `statusBufLen <= 0` is not guarded before being passed to `strlcpy` as `size_t`; truncation is not surfaced.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1859`, the definition in `n_helpers.c:1368`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that want the Notecard network status copied into a caller-provided buffer.

## Tests Observed

Focused tests in `test/src/NoteGetNetStatus_test.cpp` fake `NoteNewRequest` and `NoteRequestResponse`. Covered cases: no response returns false, response with `"err"` returns false, and response with `"status"` returns true and copies the value. Gaps include request argument assertions, output clearing on failure, truncation, invalid buffer lengths, and real transport behavior.

## Graph Links

- Graph node id: `function:n_helpers.c:1368:NoteGetNetStatus`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `NoteRequestResponse`, `NoteResponseError`, `JGetString`, `strlcpy`, and `NoteDeleteResponse`.

## Evidence Gaps

No unresolved source gaps for this node.
