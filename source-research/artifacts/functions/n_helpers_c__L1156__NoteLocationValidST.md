# `NoteLocationValidST`

## Status

Finalized. Source-inspected time-suppressed location validity helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1156`
- Declaration/interface: `note.h:1774`
- Signature/API shape: `bool NoteLocationValidST(char *errbuf, uint32_t errbuflen)`
- Visibility: public SDK API
- Public/exported names: `NoteLocationValidST`
- Macro aliases/wrappers: none
- Ownership/lifetime role: writes optional caller error buffer; owns and deletes response documents it receives

## Dependency Status

- Project-local dependencies:
  - `function:n_str.c:31:strlcpy` finalized
  - `function:n_helpers.c:2301:_timerExpiredSecs` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `macro:note.h:486:NoteDeleteResponse` finalized
- External/system dependencies: `strcmp`
- Data/type/global dependencies: `locationTimer`, `locationValid`, `locationLastErr`, `suppressionTimerSecs`

## Behavior

If `errbuf` is non-null, copies the cached `locationLastErr` into it at entry. If `locationValid` is already true, it clears cached and caller-visible error text and returns true without making another request.

When not already valid, it calls `_timerExpiredSecs(&locationTimer, suppressionTimerSecs)`. If the timer has not expired, it returns false after leaving the entry-copied cached error in `errbuf`.

When the timer permits polling, it sends `NoteRequestResponse(NoteNewRequest("card.location"))`. A null response returns false. A non-error response, or an error response whose `"mode"` field is `"off"`, is treated as valid: the response is deleted, `locationValid` becomes true, cached and caller-visible error text are cleared, and the function returns true.

For other error responses, it copies the response `"err"` field into `locationLastErr`, mirrors it into `errbuf` when provided, deletes the response, and returns false.

## Important Boundaries

- Preconditions and assumptions: `errbuf` may be null; if non-null, `errbuflen` must describe writable storage.
- Error/null/empty behavior: null response returns false and leaves any entry-copied cached error unchanged; location mode `"off"` is treated as valid even if the response also has an error field.
- Ownership and lifetime: consumes and deletes the response returned by `NoteRequestResponse`; does not own caller buffers.
- Concurrency/global state: mutates static cached location validity/error/timer state; no explicit locking.
- Performance shape: O(1) plus optional Notecard transaction when the suppression timer expires.
- Portability/platform constraints: timing depends on `_timerExpiredSecs` and the platform millisecond hook; Notecard I/O remains behind request/transaction hooks.

## Direct Call Sites

Exact source search found the non-suppressed public wrapper `NoteLocationValid` as the production caller. `NoteLocationValid` resets `locationTimer`, clears `locationValid` and `locationLastErr`, then calls this suppressed variant. The public header also declares `NoteLocationValidST` directly.

## Role In Callers

Provides the reusable, cached implementation for location validity checks. The wrapper forces an immediate fresh check, while direct `NoteLocationValidST` callers can reuse cached valid/error state until the suppression timer allows another `card.location` request.

## Tests Observed

`test/src/NoteLocationValid_test.cpp` covers no response, error response with error-buffer propagation, `"mode": "off"` treated as valid, and the cached-valid path where a subsequent `NoteLocationValidST` call does not make another `NoteRequestResponse` call.

## Graph Links

- Graph node id: `function:n_helpers.c:1156:NoteLocationValidST`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
