# `NoteSetUploadMode`

## Status

Finalized. Source-inspected public upload-mode request helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1993`
- Declaration/interface: `note.h:2020`
- Public/exported names: `NoteSetUploadMode`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteSetUploadMode(const char *uploadMode, int uploadMinutes, bool align)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: constructs a `hub.set` request and transfers it to `NoteRequest`

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_cjson.c:1974:JAddNumberToObject` finalized
  - `function:n_cjson.c:1950:JAddBoolToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
- External/system dependencies: Notecard/Notehub transaction path through `NoteRequest`
- Data/type/global dependencies: transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Creates a `hub.set` request. If request allocation succeeds, adds `"mode"` from `uploadMode`; when `uploadMinutes != 0`, adds `"outbound"` and `"align"`; then returns `NoteRequest(req)`. If request allocation fails, returns false without calling `NoteRequest`.

## Important Boundaries

- Preconditions and assumptions: `uploadMode` is passed directly to `JAddStringToObject`; a null mode is not locally rejected, and the finalized callee returns null for null strings while this helper ignores that result.
- Error/null/empty behavior: request allocation failure returns false. Zero upload minutes omits both `"outbound"` and `"align"`. JSON field-add return values are ignored, so field allocation/attach failures do not stop `NoteRequest`.
- Ownership and lifetime: request ownership transfers to `NoteRequest`, which handles request/response cleanup through its finalized path. `JAddStringToObject` copies the mode string rather than retaining the caller pointer.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: one request allocation, up to three JSON field additions, and one request transaction.
- Security or parsing constraints: does not validate upload mode tokens or reject negative interval values; non-zero negative intervals are emitted as JSON numbers.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:2020`, the definition in `n_helpers.c:1993`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that configure Notehub upload mode and optional upload interval alignment.

## Tests Observed

Focused tests in `test/src/NoteSetUploadMode_test.cpp` fake `NoteNewRequest` and `NoteRequest`. Covered cases: request allocation failure; `NoteRequest` false/true propagation; zero minutes omitting `"outbound"` and `"align"`; non-zero minutes adding both. Gaps include request name, `"mode"` value, `align=false`, null `uploadMode`, negative minutes, and JSON field-add failures.

## Graph Links

- Graph node id: `function:n_helpers.c:1993:NoteSetUploadMode`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddStringToObject`, `JAddNumberToObject`, `JAddBoolToObject`, and `NoteRequest`.

## Evidence Gaps

No unresolved source gaps for this node.
