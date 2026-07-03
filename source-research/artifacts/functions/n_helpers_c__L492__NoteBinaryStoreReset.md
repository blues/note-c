# `NoteBinaryStoreReset`

## Status

Finalized. Source-inspected public binary-store reset helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:492`
- Declaration/interface: `note.h:1670`
- Public/exported names: `NoteBinaryStoreReset`
- Macro aliases/wrappers: none found
- Signature/API shape: `const char * NoteBinaryStoreReset(void)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: constructs a `card.binary` request and consumes/deletes the response returned by `NoteRequestResponse`

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:1950:JAddBoolToObject` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
- External/system dependencies: Notecard `card.binary` request/response transaction path through `NoteRequestResponse`
- Data/type/global dependencies: transitive request/transaction and log-hook globals through callees
- Artifact coverage: source-inspected artifact

## Behavior

Creates a `card.binary` request. If request allocation succeeds, adds `"delete": true` and sends the request with `NoteRequestResponse`. If `NoteResponseError(rsp)` is true, including the null-response case, it logs the response `"err"` string, deletes the response, returns an `ERRSTR("failed to reset binary buffer", c_err)` pointer, and logs that error string. On a non-error response, deletes the response and returns null. If request allocation fails, returns `ERRSTR("unable to allocate request", c_mem)` after logging that error.

## Important Boundaries

- Preconditions and assumptions: no input parameters.
- Error/null/empty behavior: request allocation failure returns a static error string. Null response is treated as an error through finalized `NoteResponseError`; `JGetString(NULL, "err")` returns the empty string and `JDelete(NULL)` is safe through finalized callees. `JAddBoolToObject` return value is ignored, so delete-field allocation/attach failure is only surfaced indirectly by the transaction response.
- Ownership and lifetime: request ownership transfers to `NoteRequestResponse`; any response pointer is deleted before return. Returned error strings come from `ERRSTR` and are static/configuration-dependent.
- Concurrency/global state: no direct local static state; request and logging paths use global SDK hook layers.
- Portability/platform constraints: low-memory builds use `ERRSTR` codes instead of verbose strings; transport/platform effects remain behind request/log hooks.
- Performance shape: one request allocation, one bool field addition, one request/response transaction, optional logging, and one response delete.
- Security or parsing constraints: does not inspect response fields beyond the standard `"err"` check.

## Direct Call Sites

Exact source search found the public declaration in `note.h:1670`, the definition in `n_helpers.c:492`, focused unit tests, and HITL test/support uses under `test/`. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that clear the Notecard binary store.

## Tests Observed

Focused tests in `test/src/NoteBinaryStoreReset_test.cpp` fake `NoteNewRequest` and `NoteRequestResponse`. Covered cases: request allocation failure returns an error, response error returns an error, and success response returns null. Gaps include request name, constructed `"delete": true` field, null response behavior, unchecked bool-field add failure, and real transaction/log hook behavior.

## Graph Links

- Graph node id: `function:n_helpers.c:492:NoteBinaryStoreReset`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddBoolToObject`, `NoteRequestResponse`, `NoteResponseError`, `NOTE_C_LOG_ERROR`, `JGetString`, `JDelete`, and `ERRSTR`.

## Evidence Gaps

No unresolved source gaps for this node.
