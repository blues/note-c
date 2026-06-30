# `NoteClearLocation`

## Status

Finalized. Source-inspected public location-clear helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1482`
- Declaration/interface: `note.h:1895`
- Public/exported names: `NoteClearLocation`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteClearLocation(void)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: constructs a request object and transfers it to `NoteRequest`

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:1950:JAddBoolToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
- External/system dependencies: Notecard transaction path through `NoteRequest`
- Data/type/global dependencies: transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Creates a `card.location.mode` request. If request allocation succeeds, adds `"delete": true`, then returns `NoteRequest(req)`. If request allocation fails, returns false without calling `NoteRequest`.

## Important Boundaries

- Preconditions and assumptions: no input parameters.
- Error/null/empty behavior: request allocation failure returns false. The `JAddBoolToObject` return value is ignored, so bool-field allocation/attach failure is not directly surfaced.
- Ownership and lifetime: request ownership transfers to `NoteRequest`, which handles request/response cleanup through its finalized path.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: one request allocation, one JSON boolean field addition, and one request transaction.
- Security or parsing constraints: does not locally verify the final JSON request contains `"delete": true`.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1895`, the definition in `n_helpers.c:1482`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that clear the stored Notecard location.

## Tests Observed

Focused tests in `test/src/NoteClearLocation_test.cpp` fake `NoteNewRequest` and `NoteRequest`. Covered cases: request creation failure returns false and does not call `NoteRequest`; `NoteRequest` false/true propagates. Gaps include request argument, constructed JSON fields, and bool-field add failure.

## Graph Links

- Graph node id: `function:n_helpers.c:1482:NoteClearLocation`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddBoolToObject`, and `NoteRequest`.

## Evidence Gaps

No unresolved source gaps for this node.
