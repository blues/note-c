# `NoteRequestResponseWithRetry`

## Status

Finalized. Source-inspected public retrying request/response wrapper, finalized direct callees, direct caller, timing/error boundaries, and focused tests.

## Identity

- Kind: function
- Definition: `n_request.c:234`
- Declaration/interface: `note.h:382`
- Public/exported names: `NoteRequestResponseWithRetry`
- Macro aliases/wrappers: none
- Signature/API shape: `J *NoteRequestResponseWithRetry(J *req, uint32_t timeoutSeconds)`
- Visibility: public SDK API declared in `note.h`
- Ownership/lifetime role: consumes and deletes non-null `req`; returns caller-owned response pointer when non-null

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:243:_GetMs` finalized
  - `function:n_request.c:411:NoteTransaction` finalized
  - `function:n_cjson_helpers.c:194:JContainsString` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads string constants `c_err`, `c_ioerr`, and `c_unsupported`; `_GetMs` crosses the timing hook boundary through `NoteGetMs`
- Artifact coverage: source, declaration, direct caller, finalized callees, constants, timing/error boundaries, and focused tests inspected

## Behavior

Returns `NULL` immediately when `req` is `NULL`. For non-null requests, records `startMs = _GetMs()` and computes `timeoutMs = timeoutSeconds * 1000`, then loops calling `NoteTransaction(req)`. It retries when the response is `NULL`, or when the response `"err"` field contains `c_ioerr` (`"{io}"`) and does not contain `c_unsupported` (`"{not-supported}"`). Retryable non-null responses are deleted and cleared before the next attempt. The loop exits on a non-retryable response, successful response, or elapsed timeout. After the loop, it deletes the original request with `JDelete(req)` and returns the final response pointer, which may be `NULL`.

## Important Boundaries

- Preconditions and assumptions: caller passes either `NULL` or an owning request object. `timeoutSeconds` is multiplied by 1000 in `uint32_t` without overflow checks. A zero timeout still permits one transaction attempt before the elapsed-time check can break the loop.
- Error/null/empty behavior: null request returns `NULL` and is not deleted; null transaction responses are retryable until timeout; retryable non-null error responses are deleted internally; non-`{io}` errors and responses containing both `{io}` and `{not-supported}` are returned to the caller without retry.
- Ownership and lifetime: consumes/deletes the original non-null request exactly once after the retry loop. Returned non-null response ownership transfers to the caller. Intermediate retry responses do not escape.
- Concurrency/global state: transaction orchestration, locking, reset, transport, and parser behavior are inherited from finalized `NoteTransaction`; timing comes from the global `NoteGetMs` hook through `_GetMs`.
- Portability/platform constraints: platform timing and transport behavior remain behind hook/transaction layers.
- Performance shape: O(number of attempts * transaction cost); no delay is inserted directly in this wrapper between retries.
- Security or parsing constraints: retry classification is substring-based on the response error string.

## Direct Call Sites

Exact source search found one direct production caller:

- `n_request.c:207` `NoteRequestWithRetry` calls this wrapper, returns false on `NULL`, otherwise checks the final response error field with `JIsNullString`, deletes the response, and returns a bool.

Public/test references also include the declaration in `note.h:382`, a `note.h` see-also reference, focused unit tests, and HITL tests under `test/hitl/card.binary`.

## Role In Callers

`NoteRequestWithRetry` relies on this function for the retry loop and request ownership transfer, then reduces the returned response to a bool and consumes the response. This makes `NoteRequestWithRetry` the bool-returning public companion to this response-returning retry API.

## Tests Observed

`test/src/NoteRequestResponseWithRetry_test.cpp` fakes `NoteTransaction` and `NoteGetMs` to cover null request, null response retry until timeout, retry on `{io}`, no retry on `{not-supported}`, no retry on a combined `{io} {not-supported}` error, no retry on other errors, success on the first attempt, and success on the second attempt. HITL card-binary tests invoke this API for integration-style request retries.

## Test Coverage Notes

Focused tests do not assert deletion of the original request, deletion of intermediate retry responses, overflow of `timeoutSeconds * 1000`, or behavior when the timing hook is unset/non-advancing with a nonzero timeout.

## Graph Links

- Graph node id: `function:n_request.c:234:NoteRequestResponseWithRetry`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
