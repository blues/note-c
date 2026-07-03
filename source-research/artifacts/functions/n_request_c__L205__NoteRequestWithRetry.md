# `NoteRequestWithRetry`

## Status

Finalized. Source-inspected public boolean retry request wrapper.

## Identity

- Kind: function
- Definition: `n_request.c:205`
- Declaration/interface: `note.h:459`
- Public/exported names: `NoteRequestWithRetry`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteRequestWithRetry(J *req, uint32_t timeoutSeconds)`
- Visibility: public note-c request API implemented in `n_request.c`
- Ownership/lifetime role: delegates ownership of the request to `NoteRequestResponseWithRetry` and consumes/frees the returned response

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:234:NoteRequestResponseWithRetry` finalized
  - `function:n_cjson_helpers.c:152:JIsNullString` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads `c_err`; delegated retry path uses transaction and timing state through `NoteRequestResponseWithRetry`
- Artifact coverage: source-inspected artifact with verified direct call edges

## Behavior

Calls `NoteRequestResponseWithRetry(req, timeoutSeconds)`. If that returns `NULL`, returns false. Otherwise checks whether the response has no effective `"err"` string using `JIsNullString`, deletes the response with `JDelete`, and returns the success boolean.

## Important Boundaries

- Preconditions and assumptions: `req` is either null or a valid mutable `J *` request object accepted by `NoteRequestResponseWithRetry`.
- Error/null/empty behavior: null request or exhausted retry path yields `NULL` from the delegate and false here; non-null response with an `"err"` string yields false.
- Ownership and lifetime: the delegated function always frees the request object; this wrapper always deletes any response it receives.
- Concurrency/global state: no local locking; transaction locking and retry timing are delegated.
- Portability/platform constraints: wrapper is transport-independent.
- Performance shape: one delegated retry operation plus one response error check.
- Security or parsing constraints: trusts the delegated response object and checks only the `"err"` field.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:459`, the definition in `n_request.c:205`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public convenience API for downstream code that only needs success/failure and not the response body.

## Tests Observed

Focused tests in `test/src/NoteRequestWithRetry_test.cpp` cover null requests, timeout after null responses, timeout after I/O error responses, millisecond counter overflow, non-I/O error response, first-try success, and success after retry. Gap: the wrapper test does not directly assert the not-supported no-retry branch, which is covered in the delegated response-with-retry behavior.

## Graph Links

- Graph node id: `function:n_request.c:205:NoteRequestWithRetry`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing call edges to `NoteRequestResponseWithRetry`, `JIsNullString`, and `JDelete`.

## Evidence Gaps

No unresolved source gaps for this node.
