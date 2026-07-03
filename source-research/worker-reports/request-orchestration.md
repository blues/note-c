# Request Orchestration Worker Report

## Scope

- Worker: `019f1417-99a2-7212-8bef-20b835e606dc`
- Report status: merged as source-backed inspected evidence
- Source policy: source-only; focused support evidence only
- Primary file inspected: `n_request.c:1-1167`
- Supporting source inspected: `note.h:320-524`, `note.h:1038-1057`, `note.h:1178-1210`, `note.h:1258-1270`, `note.h:1308-1339`, `n_lib.h:38-45`, `n_lib.h:84-98`, `n_lib.h:120-150`, `n_lib.h:168-246`, `n_hooks.c:657-700`, `n_hooks.c:1024-1048`, `n_helpers.c:376-430`, `n_helpers.c:636-686`, `n_cjson_helpers.c:152-214`, `n_cjson.h:185-205`.

## Evidence Summary

- `NoteNewRequest` and `NoteNewCommand` allocate a `J` object and add `"req"` or `"cmd"` respectively. Allocation failure returns `NULL`; request string content is not explicitly validated at construction.
- `NoteRequest` calls `NoteRequestResponse`, returns `false` for `NULL`, otherwise succeeds only when response field `"err"` is null, missing, or empty, then deletes the response.
- `NoteRequestResponse` returns `NULL` for null input, otherwise calls `NoteTransaction(req)`, always deletes `req`, and returns the response object.
- `NoteRequestResponseWithRetry` repeats `NoteTransaction(req)` until it gets a non-null response without retryable `{io}` error or until the timeout expires. It deletes interim retry responses and deletes the request at the end.
- `NoteTransaction` delegates to `_noteTransactionShouldLock(req, true)`.
- `_noteTransactionShouldLock` serializes the request, validates that exactly one of `"req"` or `"cmd"` is present, extracts `"id"`, starts a transaction, calculates timeout, optionally locks the Notecard, captures the current CRC sequence number, optionally adds CRC for requests, resets if required, sends newline-terminated JSON through `_Transaction`, parses response JSON, classifies retryable errors, advances `seqNo` before unlock, and returns a parsed response, error object, empty command response object, or `NULL`.
- `NoteRequestResponseJSON` is a separate raw string path. The caller retains request-string ownership. The function starts a transaction, locks once, tokenizes newline-delimited command pipelines, sends commands without response capture, sends requests with `char **response`, builds newline-terminated JSON error strings on transport error, unlocks/stops, and returns a caller-owned response string or `NULL`.

## Verified Searches

- `_noteTransactionShouldLock` production callers: `n_request.c:413`, `n_helpers.c:392`, and `n_helpers.c:653`. The helper callers already hold `_LockNote()` and pass `false`.
- `NoteTransaction` production callers: `n_request.c:228`, `n_request.c:250`, plus declaration/definition.
- Retry/raw JSON APIs `NoteRequestWithRetry`, `NoteRequestResponseWithRetry`, `NoteRequestResponseJSON`, and `NoteSetRequestTimeout` have no non-test production callers beyond declarations, definitions, and wrappers.
- `_Transaction` maps to `_noteJSONTransaction` via `n_lib.h:238`. `_noteJSONTransaction` dispatches to active `notecardTransaction` or returns `"a valid interface must be selected"` at `n_hooks.c:1042`.
- No `NoteDeleteRequest` symbol was found; request deletion is performed through `JDelete(req)` or wrapper ownership.

## Ownership And Error Boundaries

- `NoteRequestResponse` and `NoteRequestResponseWithRetry` consume and delete their input `J *req`.
- `NoteTransaction` and `_noteTransactionShouldLock` do not delete `req`; caller retains ownership.
- `NoteRequest` and `NoteRequestWithRetry` delete returned response objects internally.
- `NoteRequestResponse` returns caller-owned `J *rsp`; public deletion macro is `NoteDeleteResponse(rsp) -> JDelete(rsp)`.
- `NoteRequestResponseJSON` does not free caller `reqJSON`; it frees only temporary newline copies. Returned response strings are dynamically allocated and caller-owned.
- `NoteResponseError(NULL)` evaluates true because `JIsNullString(NULL, ...)` returns false; callers generally check `rsp` first.
- `NoteResponseErrorContains(NULL, ...)` evaluates false through `JContainsString`.
- `NoteErrorContains` and `NoteErrorClean` have no null guards.
- `NoteSetRequestTimeout` stores a global override; `CARD_INTER_TRANSACTION_TIMEOUT_SEC` is 30 unless overridden.
- `_noteTransaction_calculateTimeoutMs` defaults to the inter-transaction timeout, honors `"milliseconds"` or `"seconds"` on `note.add` and `web.*`, then adds 1000 ms. A source comment says `web.*` defaults to 90 seconds, but the implementation does not set 90 seconds.
- `hub.set` user-agent body is added after serialization, so the inspected source does not show that body being sent on the wire.

## Recommended Claims

- Record the main request chain: `NoteRequest -> NoteRequestResponse -> NoteTransaction -> _noteTransactionShouldLock -> _noteJSONTransaction`.
- Record `NoteRequestResponseJSON` as a separate raw JSON/string API path with caller-owned response strings.
- Record request ownership split: `NoteRequest*` wrappers consume `J *req`; `NoteTransaction` does not.
- Record binary helper lock-bypass callers as intentional `_noteTransactionShouldLock(..., false)` uses.
- Record caveats for unimplemented `web.*` 90-second default and `hub.set` user-agent serialization ordering.
