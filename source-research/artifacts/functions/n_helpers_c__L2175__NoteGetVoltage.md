# `NoteGetVoltage`

## Status

Finalized. Source-inspected public voltage read helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:2175`
- Declaration/interface: `note.h:2065`
- Public/exported names: `NoteGetVoltage`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteGetVoltage(JNUMBER *voltage)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: initializes caller-owned output storage, receives a response object from `NoteRequestResponse`, and deletes the response with `NoteDeleteResponse`

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_cjson_helpers.c:99:JGetNumber` finalized
  - `macro:note.h:486:NoteDeleteResponse` finalized
- External/system dependencies: Notecard request/response transaction path through `NoteRequestResponse`
- Data/type/global dependencies: `JNUMBER` precision depends on public header configuration; transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Writes `0.0` to `*voltage`, sends a `card.voltage` request through `NoteRequestResponse(NoteNewRequest(...))`, and returns false when no response is returned. When a response exists, returns false if `NoteResponseError(rsp)` reports an error. On a non-error response, assigns `JGetNumber(rsp, "value")` to `*voltage`, deletes the response, and returns true.

## Important Boundaries

- Preconditions and assumptions: `voltage` must be non-null and writable because the function writes `*voltage` before request allocation.
- Error/null/empty behavior: request/response failure and response errors return false after the output has been reset to zero. A no-error response with missing or non-number `"value"` returns true with `0.0`, following finalized `JGetNumber` behavior.
- Ownership and lifetime: `NoteRequestResponse` owns the request created by `NoteNewRequest`; the response object is deleted by `NoteDeleteResponse` on every non-null response path.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: `JNUMBER` is `float` under `NOTE_C_SINGLE_PRECISION` and `double` otherwise; no platform-specific behavior in this wrapper.
- Performance shape: one request allocation/transaction, one response error check, one numeric field read, and one response delete.
- Security or parsing constraints: missing/non-numeric `"value"` is indistinguishable from a real numeric zero on successful no-error responses.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:2065`, the definition in `n_helpers.c:2175`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that retrieve the Notecard voltage reading.

## Tests Observed

Focused tests in `test/src/NoteGetVoltage_test.cpp` fake `NoteNewRequest` and `NoteRequestResponse`. Covered cases: request creation failure, no response, response error, and successful numeric `"value"` copy. Gaps include null output pointer, request name, missing/non-number `"value"`, response cleanup assertion, and real transaction hook behavior.

## Graph Links

- Graph node id: `function:n_helpers.c:2175:NoteGetVoltage`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `NoteRequestResponse`, `NoteResponseError`, `JGetNumber`, and `NoteDeleteResponse`.

## Evidence Gaps

No unresolved source gaps for this node.
