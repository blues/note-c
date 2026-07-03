# `NoteGetLocation`

## Status

Finalized. Source-inspected public `card.location` helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1417`
- Declaration/interface: `note.h:1880`
- Public/exported names: `NoteGetLocation`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteGetLocation(JNUMBER *retLat, JNUMBER *retLon, JTIME *time, char *statusBuf, int statusBufLen)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: writes optional caller-owned output slots and owns/deletes the response object it receives

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_str.c:31:strlcpy` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `function:n_cjson_helpers.c:19:JIsPresent` finalized
  - `function:n_cjson_helpers.c:99:JGetNumber` finalized
  - `function:n_cjson_helpers.c:122:JGetInt` finalized
  - `macro:note.h:486:NoteDeleteResponse` expands to finalized `JDelete`
- External/system dependencies: Notecard transaction path through `NoteRequestResponse`
- Data/type/global dependencies: transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Initializes non-null outputs before I/O: clears `statusBuf`, sets `*retLat = 0.0`, `*retLon = 0.0`, and `*time = 0`. Sends `card.location` via `NoteRequestResponse(NoteNewRequest("card.location"))`. For a non-null response, copies `"status"` to `statusBuf` when provided, sets `locValid = true` only when both `"lat"` and `"lon"` fields are present, copies numeric latitude/longitude into non-null output pointers, copies nonzero `"time"` into `time`, deletes the response, and returns `locValid`.

## Important Boundaries

- Preconditions and assumptions: output pointers are optional except that `statusBufLen` should describe `statusBuf` capacity when `statusBuf` is non-null.
- Error/null/empty behavior: no response returns false after output initialization. Response `"err"` is not checked; success depends on presence of both `"lat"` and `"lon"`.
- Ownership and lifetime: caller owns output storage; `NoteRequestResponse` owns/frees the request, and this helper deletes the returned response.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: uses `JNUMBER` and `JTIME` public typedefs and the bundled or platform `strlcpy`.
- Performance shape: one request/response transaction plus field lookups and bounded buffer copy.
- Security or parsing constraints: `statusBufLen <= 0` is not guarded before being passed to `strlcpy` as `size_t`; non-number `lat`/`lon` fields still make `locValid` true because presence, not numeric type, gates success.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1880`, the definition in `n_helpers.c:1417`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that want current Notecard location values copied into optional output slots.

## Tests Observed

Focused tests in `test/src/NoteGetLocation_test.cpp` fake `NoteNewRequest` and `NoteRequestResponse`. Covered cases: no response returns false, and a response with status/lat/lon/time returns true and populates outputs. Gaps include request string assertions, null output pointers, zero/negative `statusBufLen`, `"err"` responses, missing lat/lon, and non-number lat/lon.

## Graph Links

- Graph node id: `function:n_helpers.c:1417:NoteGetLocation`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `NoteRequestResponse`, `strlcpy`, `JGetString`, `JIsPresent`, `JGetNumber`, `JGetInt`, and `NoteDeleteResponse`.

## Evidence Gaps

No unresolved source gaps for this node.
