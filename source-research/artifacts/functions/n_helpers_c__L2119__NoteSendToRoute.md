# `NoteSendToRoute`

## Status

Finalized. Source-inspected public route-send helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:2119`
- Declaration/interface: `note.h:2057`
- Public/exported names: `NoteSendToRoute`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteSendToRoute(const char *method, const char *routeAlias, char *notefile, J *body)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: consumes caller-owned `body`, first through a `note.event` request and then by detaching the response body into a `web.<method>` request

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson.c:1866:JAddItemToObject` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `macro:note.h:486:NoteDeleteResponse` finalized
  - `function:n_cjson.c:2135:JDetachItemFromObject` finalized
  - `function:n_str.c:31:strlcpy` finalized
  - `function:n_str.c:69:strlcat` finalized
  - `function:n_request.c:191:NoteRequest` finalized
- External/system dependencies: Notecard `note.event` request path and dynamic `web.<method>` request path through transaction hooks
- Data/type/global dependencies: JSON object ownership and transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Builds a two-step route send. First creates a `note.event` request; if allocation fails, deletes the caller-supplied `body` and returns false. It attaches `body` as `"body"`, adds `"file"` from `notefile`, and sends the first request with `NoteRequestResponse`. A null response or response error returns false after any non-null response cleanup. On success, detaches `"body"` from the response and deletes the response. It then builds a stack request string starting with `"web."` plus `method`, creates that request, deletes the detached body and returns false if allocation fails, otherwise attaches the body, adds `"route"` from `routeAlias`, and returns `NoteRequest(req)`.

## Important Boundaries

- Preconditions and assumptions: `method` must be non-null because it is passed to `strlcat`. `notefile` and `routeAlias` are passed to `JAddStringToObject`; null values cause that helper to return null, which this function ignores.
- Error/null/empty behavior: first request allocation failure deletes the original `body`. Null first response returns false. Error response is deleted and returns false. Missing response `"body"` produces a null detached body; the second request can still be sent and succeed if `NoteRequest` succeeds. Second request allocation failure deletes the detached body.
- Ownership and lifetime: caller gives up ownership of `body`. On the first request path, `JAddItemToObject` transfers `body` into the `note.event` request; the response body is detached into local ownership, then transferred into the `web.<method>` request or deleted on second allocation failure. Request and response objects are consumed by finalized request/delete paths.
- Concurrency/global state: no direct global state; both transaction paths use request state, locks, transport hooks, allocation hooks, timing hooks, reset state, and optional CRC state.
- Portability/platform constraints: builds the dynamic request string in a fixed 32-byte stack buffer using project-local `strlcpy`/`strlcat`; no platform-specific code in this wrapper.
- Performance shape: two sequential Notecard transactions on the success path, plus JSON attach/detach work and bounded request-string construction.
- Security or parsing constraints: long `method` values are silently truncated; request-string truncation is not reported. The helper does not validate `method` against `get`, `post`, or `put`, does not verify `"file"` or `"route"` field attachment, and can send a second request without a body.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:2057`, the definition in `n_helpers.c:2119`, and focused tests. No production repository caller or alias was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that convert a note body into an event and send it to a Notehub route via a dynamic web request.

## Tests Observed

Focused tests in `test/src/NoteSendToRoute_test.cpp` fake `NoteNewRequest`, `NoteRequestResponse`, and `NoteRequest`. Covered cases: first request allocation failure, null first response, error response, second request allocation failure, final request false/true, and `"route"` value on the second request. Gaps include first request name, second request name, `"file"` field, event body detach/forwarding, missing response body behavior, null arguments, method truncation, and unchecked JSON add failures.

## Graph Links

- Graph node id: `function:n_helpers.c:2119:NoteSendToRoute`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JDelete`, `JAddItemToObject`, `JAddStringToObject`, `NoteRequestResponse`, `NoteResponseError`, `NoteDeleteResponse`, `JDetachItemFromObject`, `strlcpy`, `strlcat`, and `NoteRequest`.

## Evidence Gaps

No unresolved source gaps for this node.
