# `NoteSetLocationMode`

## Status

Finalized. Source-inspected public location-mode write helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1524`
- Declaration/interface: `note.h:1913`
- Public/exported names: `NoteSetLocationMode`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteSetLocationMode(const char *mode, uint32_t seconds)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: constructs a request object and transfers it to `NoteRequest`

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_cjson.c:1974:JAddNumberToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
- External/system dependencies: Notecard transaction path through `NoteRequest`
- Data/type/global dependencies: transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Creates a `card.location.mode` request. If request allocation succeeds, rewrites an empty `mode` string to `"-"`, adds `"mode"` and `"seconds"` fields, and returns `NoteRequest(req)`. If request allocation fails, returns false without calling `NoteRequest`.

## Important Boundaries

- Preconditions and assumptions: `mode` must be non-null when request allocation succeeds because the function reads `mode[0]` before calling any guarded JSON helper.
- Error/null/empty behavior: request allocation failure returns false. Empty string mode is converted to `"-"`. `JAddStringToObject` and `JAddNumberToObject` return values are ignored, so field allocation/attach failures are not surfaced before `NoteRequest`.
- Ownership and lifetime: request ownership transfers to `NoteRequest`, which handles request/response cleanup through its finalized path.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: one request allocation, two JSON field additions, and one request transaction.
- Security or parsing constraints: does not locally validate allowed mode values or verify that both request fields were attached.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1913`, the definition in `n_helpers.c:1524`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that set the Notecard location mode and periodic interval.

## Tests Observed

Focused tests in `test/src/NoteSetLocationMode_test.cpp` fake `NoteNewRequest` and `NoteRequest`. Covered cases: request creation failure returns false and does not call `NoteRequest`; `NoteRequest` false/true propagates; empty mode is encoded as `"-"`. Gaps include null mode and JSON field add failures.

## Graph Links

- Graph node id: `function:n_helpers.c:1524:NoteSetLocationMode`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddStringToObject`, `JAddNumberToObject`, and `NoteRequest`.

## Evidence Gaps

No unresolved source gaps for this node.
