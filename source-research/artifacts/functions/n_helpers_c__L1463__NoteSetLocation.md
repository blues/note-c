# `NoteSetLocation`

## Status

Finalized. Source-inspected public fixed-location setter.

## Identity

- Kind: function
- Definition: `n_helpers.c:1463`
- Declaration/interface: `note.h:1889`
- Public/exported names: `NoteSetLocation`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteSetLocation(JNUMBER lat, JNUMBER lon)`
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

Creates a `card.location.mode` request. If request allocation succeeds, adds `"mode": "fixed"`, `"lat": lat`, and `"lon": lon`, then returns `NoteRequest(req)`. If request allocation fails, returns false without calling `NoteRequest`.

## Important Boundaries

- Preconditions and assumptions: latitude and longitude are passed by value with no source-level range validation.
- Error/null/empty behavior: request allocation failure returns false. Individual JSON field-add return values are ignored, so partial request construction failure is not directly surfaced.
- Ownership and lifetime: request ownership transfers to `NoteRequest`, which handles request/response cleanup through its finalized path.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: uses `JNUMBER`, whose precision depends on build configuration.
- Performance shape: one request allocation, three JSON field additions, and one request transaction.
- Security or parsing constraints: no validation of NaN/Inf, coordinate ranges, or Notecard-side acceptance in this helper.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1889`, the definition in `n_helpers.c:1463`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that set a fixed Notecard location.

## Tests Observed

Focused tests in `test/src/NoteSetLocation_test.cpp` fake `NoteNewRequest` and `NoteRequest`. Covered cases: request creation failure returns false and does not call `NoteRequest`; `NoteRequest` false/true propagates. Gaps include request argument, constructed JSON fields, field-add failure, and numeric edge cases.

## Graph Links

- Graph node id: `function:n_helpers.c:1463:NoteSetLocation`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddStringToObject`, `JAddNumberToObject`, and `NoteRequest`.

## Evidence Gaps

No unresolved source gaps for this node.
