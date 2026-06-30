# `NoteGetEnv`

## Status

Finalized. Source-inspected public environment string retrieval helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1302`
- Declaration/interface: `note.h:1811`
- Signature/API shape: `bool NoteGetEnv(const char *variable, const char *defaultVal, char *buf, uint32_t buflen)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NoteGetEnv`
- Macro aliases/wrappers: none
- Ownership/lifetime role: writes caller output buffer; consumes request through `NoteRequestResponse`; deletes response after inspection

## Dependency Status

- Project-local dependencies:
  - `function:n_str.c:31:strlcpy` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `macro:note.h:486:NoteDeleteResponse` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: sends Notecard `env.get` request and reads response JSON fields
- Artifact coverage: generated artifact exists

## Behavior

Initializes `buf` to `defaultVal` with `strlcpy`, or to an empty string when `defaultVal == NULL`, before sending any request. Creates an `env.get` request with `NoteNewRequest`, adds `"name": variable`, sends it with `NoteRequestResponse`, and treats a non-null response without `NoteResponseError(rsp)` as success.

On success, reads `JGetString(rsp, "text")`; only a non-empty returned string replaces the default already in `buf`. Missing, non-string, null, or empty `"text"` leaves the default/empty buffer intact. Any non-null response is released with `NoteDeleteResponse` before return.

## Important Boundaries

- Preconditions and assumptions: `buf` must be non-null and usable before any request work; the function writes `buf[0]` when `defaultVal == NULL`. `variable` should be non-null for `"name"` attachment, but `JAddStringToObject` failure is not checked.
- Error/null/empty behavior: request allocation failure, null response, or response error returns `false` with the initialized default value left in `buf`. A no-error response with missing/empty `"text"` returns `true` and still leaves the initialized default value.
- Ownership and lifetime: caller owns `buf`; `NoteRequestResponse` consumes the request; this function owns and deletes the returned response after borrowing the `"text"` pointer.
- Concurrency/global state: uses the SDK request/transaction path and allocator hooks behind callees.
- Portability/platform constraints: platform I/O remains behind request/transport hooks.
- Performance shape: one request/response transaction plus bounded output copy into `buf`.
- Security or parsing constraints: no explicit output-buffer null check; `strlcpy` bounds copies by `buflen` in non-null-default paths.

## Direct Call Sites

Exact source search found direct production callers:

- `n_helpers.c:1269` `NoteGetEnvNumber` formats a numeric default, calls `NoteGetEnv`, and parses the resulting buffer.
- `n_helpers.c:1285` `NoteGetEnvInt` formats an integer default, calls `NoteGetEnv`, and parses the resulting buffer.

## Role In Callers

This is the shared string retrieval layer for numeric and integer environment wrappers. The wrappers ignore its boolean return and rely on the initialized output buffer.

## Tests Observed

Focused `test/src/NoteGetEnv_test.cpp` covers request allocation failure, null response, response error, missing `"text"` with success/default retention, and valid `"text"` overriding the default.

## Graph Links

- Graph node id: `function:n_helpers.c:1302:NoteGetEnv`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `strlcpy`, `NoteNewRequest`, `JAddStringToObject`, `NoteRequestResponse`, `NoteResponseError`, `JGetString`, and `NoteDeleteResponse`.

## Evidence Gaps

No unresolved source gaps for this node.
