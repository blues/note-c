# `NoteBinaryStoreDecodedLength`

## Status

Finalized. Source-inspected public binary-store decoded-length helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:249`
- Declaration/interface: `note.h:1644`
- Public/exported names: `NoteBinaryStoreDecodedLength`
- Macro aliases/wrappers: none found
- Signature/API shape: `const char * NoteBinaryStoreDecodedLength(uint32_t *len)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: writes caller-owned output storage, creates a `card.binary` request, owns/deletes the returned response, and returns static/configuration-dependent error strings

## Dependency Status

- Project-local dependencies:
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `function:n_request.c:940:NoteErrorContains` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson_helpers.c:122:JGetInt` finalized
- External/system dependencies: Notecard `card.binary` request/response transaction path through `NoteRequestResponse`
- Data/type/global dependencies: constants `c_bad`, `c_badbinerr`, and `c_err`; transitive request/transaction and log-hook globals through callees
- Artifact coverage: source-inspected artifact

## Behavior

Validates that `len` is non-null. If it is null, logs and returns `ERRSTR("len cannot be NULL", c_bad)`. Otherwise sends a `card.binary` request via `NoteRequestResponse(NoteNewRequest(...))`. A null response logs and returns `ERRSTR("unable to issue binary request", c_err)`. If the response contains an error, it reads `"err"` and swallows only errors containing `c_badbinerr`; other errors are logged, the response is deleted, and `ERRSTR("unexpected error received during handshake", c_bad)` is returned. On success or swallowed `{bad-bin}`, writes `JGetInt(rsp, "length")` into `*len`, deletes the response, and returns null.

## Important Boundaries

- Preconditions and assumptions: `len` must be non-null to receive output; null is rejected before request work.
- Error/null/empty behavior: null response and non-`{bad-bin}` response errors return static/configuration-dependent error strings. Missing or non-number `"length"` becomes zero through finalized `JGetInt`. `{bad-bin}` is intentionally treated as success and still reads `"length"`.
- Ownership and lifetime: `NoteRequestResponse` owns/deletes the request. This helper owns any non-null response and deletes it before returning. Returned error strings are not caller-owned.
- Concurrency/global state: no direct local static state; request and logging paths use global SDK hook layers.
- Portability/platform constraints: low-memory builds use compact `ERRSTR` codes instead of verbose strings.
- Performance shape: one request/response transaction, one response error check, optional error string scan, one integer field read, and response cleanup.
- Security or parsing constraints: no range/sign validation before assigning `JGetInt` to `uint32_t`; negative JSON integers convert according to C assignment rules.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1644`, the definition in `n_helpers.c:249`, and focused tests. No production repository caller or alias was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that query the decoded length currently stored in the Notecard binary store.

## Tests Observed

Focused tests in `test/src/NoteBinaryStoreDecodedLength_test.cpp` fake `NoteRequestResponse`. Covered cases: null output pointer, null response, generic response error, `"length": 0`, and positive `"length"`. Gaps include swallowed `{bad-bin}`, request shape, missing/non-number `"length"`, `NoteNewRequest` allocation/add-field failure, and response cleanup/log assertions.

## Graph Links

- Graph node id: `function:n_helpers.c:249:NoteBinaryStoreDecodedLength`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `ERRSTR`, `NOTE_C_LOG_ERROR`, `NoteNewRequest`, `NoteRequestResponse`, `NoteResponseError`, `JGetString`, `NoteErrorContains`, `JDelete`, and `JGetInt`.

## Evidence Gaps

No unresolved source gaps for this node.
