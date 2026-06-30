# `NoteGetContact`

## Status

Finalized. Source-inspected public contact read helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:2226`
- Declaration/interface: `note.h:2088`
- Public/exported names: `NoteGetContact`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteGetContact(char *nameBuf, int nameBufLen, char *orgBuf, int orgBufLen, char *roleBuf, int roleBufLen, char *emailBuf, int emailBufLen)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: initializes caller-owned output buffers when non-null, receives a response object from `NoteRequestResponse`, and deletes the response with `NoteDeleteResponse`

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

Clears each non-null output buffer to an empty string, sends a `card.contact` request through `NoteRequestResponse(NoteNewRequest(...))`, and returns false when no response is returned. When a response exists, returns false if `NoteResponseError(rsp)` reports an error. On a non-error response, copies `"name"`, `"org"`, `"role"`, and `"email"` response fields into the corresponding non-null buffers using `strlcpy`, deletes the response, and returns true.

## Important Boundaries

- Preconditions and assumptions: null output buffers are accepted and skipped. Every non-null output buffer must be writable for at least one byte because the function writes the leading NUL before checking that buffer's length argument.
- Error/null/empty behavior: request/response failure and response errors return false after clearing all non-null buffers. Missing or non-string contact fields copy as empty strings through finalized `JGetString`.
- Ownership and lifetime: `NoteRequestResponse` owns the request created by `NoteNewRequest`; the response object is deleted by `NoteDeleteResponse` on every non-null response path. Copied strings do not escape as borrowed response pointers.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: uses project-local `strlcpy`; no platform-specific behavior in this wrapper.
- Performance shape: one request allocation/transaction, one response error check, up to four bounded string copies, and one response delete.
- Security or parsing constraints: truncation is not reported. Lengths are `int` but passed to `strlcpy` as `size_t`; negative values would convert to very large sizes.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:2088`, the definition in `n_helpers.c:2226`, and focused tests. No production repository caller or alias was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that retrieve contact fields from the Notecard.

## Tests Observed

Focused tests in `test/src/NoteGetContact_test.cpp` fake `NoteNewRequest` and `NoteRequestResponse`. Covered cases: request allocation failure, response failure, response error, successful all-null buffers, and successful copy of all four fields. Gaps include request name, truncation, zero/negative lengths, missing/non-string fields, empty `"err"`, mixed null/non-null buffers, and real transaction hook behavior.

## Graph Links

- Graph node id: `function:n_helpers.c:2226:NoteGetContact`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `NoteRequestResponse`, `NoteResponseError`, `strlcpy`, `JGetString`, and `NoteDeleteResponse`.

## Evidence Gaps

No unresolved source gaps for this node.
