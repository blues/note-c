# `NoteGetStatusST`

## Status

Finalized. Source-inspected suppressed card-status helper, static cache/timer behavior, caller, and focused wrapper tests.

## Identity

- Kind: function
- Definition: `n_helpers.c:1649`
- Declaration/interface: `note.h:1967`
- Public/exported names: `NoteGetStatusST`
- Macro aliases/wrappers: none
- Signature/API shape: `bool NoteGetStatusST(char *statusBuf, int statusBufLen, JTIME *bootTime, bool *retUSB, bool *retSignals)`
- Visibility: public note-c helper API
- Ownership/lifetime role: copies cached status fields into caller-provided outputs; consumes and deletes response objects created by request APIs

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:2301:_timerExpiredSecs` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `function:n_cjson_helpers.c:122:JGetInt` finalized
  - `function:n_cjson_helpers.c:137:JGetBool` finalized
  - `function:n_str.c:31:strlcpy` finalized
  - `macro:note.h:486:NoteDeleteResponse` finalized
- External/system dependencies: none
- Data/type/global dependencies: static `statusTimer`; function-local static `lastStatus`, `lastBootTime`, `lastUSB`, and `lastSignals`
- Artifact coverage: source, declaration, caller, static cache state, and focused tests inspected

## Behavior

Initializes local `success` to `false`. It refreshes only when `_timerExpiredSecs(&statusTimer, suppressionTimerSecs)` reports the status timer expired. On refresh, sends `card.status` through `NoteRequestResponse(NoteNewRequest("card.status"))`. If a response is returned, `success` becomes `!NoteResponseError(rsp)`.

On successful response, it caches `status` into function-local static `lastStatus`, `time` into `lastBootTime`, `usb` into `lastUSB`, and derives `lastSignals`: when `connected` is true, `lastSignals` is `JGetInt(rsp, "signals") > 0`; otherwise `lastSignals` is false. It deletes any non-null response with `NoteDeleteResponse(rsp)`.

If the timer has not expired, it performs no Notecard I/O and sets `success = true`. Regardless of success or failure, it writes cached values to any non-null output parameters and returns `success`.

## Important Boundaries

- Preconditions and assumptions: non-null `statusBuf` must be writable for `statusBufLen` bytes.
- Error/null/empty behavior: null response or error response returns `false` for a refresh attempt but still copies current cached values to requested outputs. Missing or wrong-type JSON fields collapse through helper defaults: empty string status, zero time/signals, and false booleans.
- Ownership and lifetime: `NoteRequestResponse` consumes the request object; this function owns and deletes the returned response when non-null.
- Concurrency/global state: mutates static `statusTimer` and function-local static cached values without local locking. Suppression period uses shared static `suppressionTimerSecs`.
- Portability/platform constraints: transport remains behind `NoteRequestResponse`; timer behavior depends on the platform millisecond hook through `_timerExpiredSecs`.
- Performance shape: O(status string copy length) cache-hit path; refresh path adds one Notecard transaction and JSON lookups.
- Security or parsing constraints: signal presence is deliberately gated by `connected`; positive `signals` is ignored when `connected` is false.
- Timer edge: `_timerExpiredSecs` updates `statusTimer` before request success is known. With a nonzero millisecond clock, a failed refresh can be followed by suppressed calls that return `true` with old or default cached values until the timer expires again.

## Direct Call Sites

Exact production caller found by source search:

- `n_helpers.c:1634` `NoteGetStatus` resets `statusTimer` and delegates to this suppressed variant.

## Role In Callers

`NoteGetStatus` uses this function as the implementation after forcing a refresh attempt by setting `statusTimer = 0`. It does not clear the function-local cached values.

## Tests Observed

`test/src/NoteGetStatus_test.cpp` covers the public wrapper for no response and successful responses, including connected, disconnected, and connected-with-zero-signals cases.

## Test Coverage Notes

Focused tests call the public wrapper rather than `NoteGetStatusST` directly. They do not cover the suppressed cache-hit ST path, stale-cache success after refresh failure, `NoteSetSTSecs` interaction, error responses carrying `"err"`, wrong-type response fields, request deletion, or direct verification of the `"card.status"` request string.

## Graph Links

- Graph node id: `function:n_helpers.c:1649:NoteGetStatusST`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
