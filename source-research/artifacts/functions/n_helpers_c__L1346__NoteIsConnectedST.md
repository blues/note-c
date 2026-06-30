# `NoteIsConnectedST`

## Status

Finalized. Source-inspected suppressed connectivity helper, cache behavior, caller, and focused wrapper tests.

## Identity

- Kind: function
- Definition: `n_helpers.c:1346`
- Declaration/interface: `note.h:1850`
- Public/exported names: `NoteIsConnectedST`
- Macro aliases/wrappers: none
- Signature/API shape: `bool NoteIsConnectedST(void)`
- Visibility: public note-c helper API
- Ownership/lifetime role: allocates no caller-owned data; consumes and deletes response objects created by request APIs

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:2301:_timerExpiredSecs` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_cjson_helpers.c:137:JGetBool` finalized
  - `macro:note.h:486:NoteDeleteResponse` finalized
- External/system dependencies: none
- Data/type/global dependencies: static `connectivityTimer`, `cardConnected`, and `suppressionTimerSecs` in `n_helpers.c`
- Artifact coverage: source, declaration, caller, static state, and focused tests inspected

## Behavior

Uses `_timerExpiredSecs(&connectivityTimer, suppressionTimerSecs)` to decide whether to refresh cached connectivity state. When the timer has not expired, it returns the current static `cardConnected` cache without making a request.

When the timer expires, it sends `hub.status` through `NoteRequestResponse(NoteNewRequest("hub.status"))`. If a response is returned and `NoteResponseError(rsp)` is false, it updates `cardConnected` from `JGetBool(rsp, "connected")`. It deletes any non-null response with `NoteDeleteResponse(rsp)` and returns `cardConnected`.

## Important Boundaries

- Preconditions and assumptions: global helper state is initialized in `n_helpers.c`; normal callers do not pass arguments.
- Error/null/empty behavior: null request/response, transport failure, and response error do not clear `cardConnected`; they leave the prior cached value in place. A successful response with a missing, non-bool, or false `connected` field sets or leaves `cardConnected` false through `JGetBool`.
- Ownership and lifetime: `NoteRequestResponse` consumes the request object; `NoteIsConnectedST` owns and deletes the returned response when non-null.
- Concurrency/global state: mutates static `connectivityTimer` and `cardConnected` without local locking. Suppression period uses static `suppressionTimerSecs`, defaulting to 10 seconds.
- Portability/platform constraints: request transport remains behind `NoteRequestResponse`; timer behavior depends on the platform millisecond hook through `_timerExpiredSecs`.
- Performance shape: O(1) cached path; refresh path performs one Notecard transaction and a small JSON lookup.
- Security or parsing constraints: trusts helper-level response parsing; no validation beyond `NoteResponseError` and `JGetBool`.

## Direct Call Sites

Exact production caller found by source search:

- `n_helpers.c:1337` `NoteIsConnected` resets `connectivityTimer` and delegates to `NoteIsConnectedST`.

## Role In Callers

`NoteIsConnected` uses this suppressed variant after forcing the timer expired, giving the public wrapper an immediate refresh path while preserving the response handling and cache semantics here.

## Tests Observed

`test/src/NoteIsConnected_test.cpp` covers the public wrapper for no response, error response, connected false, and connected true cases. Those tests reach `NoteIsConnectedST` through `NoteIsConnected`.

## Test Coverage Notes

Focused tests do not assert the exact `"hub.status"` request name, the timer-suppressed cached path, or the behavior where a previous true cache survives a later null/error response.

## Graph Links

- Graph node id: `function:n_helpers.c:1346:NoteIsConnectedST`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
