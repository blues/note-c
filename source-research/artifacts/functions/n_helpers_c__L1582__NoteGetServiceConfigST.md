# `NoteGetServiceConfigST`

## Status

Finalized. Source-inspected suppressed service-configuration helper, cache/timer behavior, caller, and focused wrapper tests.

## Identity

- Kind: function
- Definition: `n_helpers.c:1582`
- Declaration/interface: `note.h:1943`
- Public/exported names: `NoteGetServiceConfigST`
- Macro aliases/wrappers: none
- Signature/API shape: `bool NoteGetServiceConfigST(char *productBuf, int productBufLen, char *serviceBuf, int serviceBufLen, char *deviceBuf, int deviceBufLen, char *snBuf, int snBufLen)`
- Visibility: public note-c helper API
- Ownership/lifetime role: copies cached service fields into caller-provided buffers; consumes and deletes response objects created by request APIs

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:2301:_timerExpiredSecs` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `function:n_str.c:31:strlcpy` finalized
  - `macro:note.h:486:NoteDeleteResponse` finalized
- External/system dependencies: none
- Data/type/global dependencies: static `serviceConfigTimer`, `scProduct`, `scService`, `scDevice`, and `scSN` in `n_helpers.c`
- Artifact coverage: source, declaration, caller, static cache state, and focused tests inspected

## Behavior

Initializes local `success` to `false`. It refreshes from the Notecard when `scProduct[0] == '\0'`, `scDevice[0] == '\0'`, or `_timerExpiredSecs(&serviceConfigTimer, 4*60*60)` reports the four-hour refresh timer expired. Because the timer check is the third operand in an `||` expression, the timer helper is not called when the product or device cache is empty.

On refresh, sends `hub.get` through `NoteRequestResponse(NoteNewRequest("hub.get"))`. If a response is returned, `success` becomes `!NoteResponseError(rsp)`. On success, it caches `product` into `scProduct`, `host` into `scService`, `device` into `scDevice`, and `sn` into `scSN` using `JGetString` and `strlcpy`. It deletes any non-null response with `NoteDeleteResponse(rsp)`.

If the cache is usable and the four-hour timer has not expired, it performs no Notecard I/O and sets `success = true`. Regardless of success or failure, it copies current cached values into any non-null output buffers with `strlcpy`, then returns `success`.

## Important Boundaries

- Preconditions and assumptions: non-null output buffers must be writable for their paired length when the corresponding pointer is non-null.
- Error/null/empty behavior: null response or error response returns `false` for a refresh attempt but still copies current cached strings to requested output buffers. Missing or non-string response fields become empty strings through `JGetString`.
- Ownership and lifetime: `NoteRequestResponse` consumes the request object; this function owns and deletes the returned response when non-null. Caller output buffers remain caller-owned.
- Concurrency/global state: mutates static service config cache and timer without local locking. `NoteSetProductID` and `NoteSetSerialNumber` reset `serviceConfigTimer` but do not clear cached strings.
- Portability/platform constraints: transport remains behind `NoteRequestResponse`; timer behavior depends on the platform millisecond hook through `_timerExpiredSecs`.
- Performance shape: O(total copied string lengths) on cache-hit path; refresh path adds one Notecard transaction and JSON lookups.
- Security or parsing constraints: cache validity only checks product and device cache strings; host/service and serial-number strings do not affect refresh eligibility.

## Direct Call Sites

Exact production caller found by source search:

- `n_helpers.c:1560` `NoteGetServiceConfig` resets `serviceConfigTimer` and delegates to this suppressed variant.

## Role In Callers

`NoteGetServiceConfig` uses this function as the implementation after forcing a refresh attempt by setting `serviceConfigTimer = 0`. It does not clear cached strings, so a forced refresh that receives no response can return `false` while still copying previously cached values.

## Tests Observed

`test/src/NoteGetServiceConfig_test.cpp` covers the public wrapper for no response with empty cache, successful response caching/copying, and a subsequent no-response call that returns false while copying previously cached service configuration values.

## Test Coverage Notes

Focused tests call the public wrapper rather than `NoteGetServiceConfigST` directly. They do not cover the suppressed cache-hit ST path, four-hour expiry, timer rollback behavior, error responses carrying `"err"`, or direct verification of the `"hub.get"` request string.

## Graph Links

- Graph node id: `function:n_helpers.c:1582:NoteGetServiceConfigST`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
