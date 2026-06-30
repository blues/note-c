# `NoteSetSerialNumber`

## Status

Finalized. Source-inspected public serial-number setter helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1963`
- Declaration/interface: `note.h:2002`
- Public/exported names: `NoteSetSerialNumber`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteSetSerialNumber(const char *sn)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: constructs a `hub.set` request and transfers it to `NoteRequest`; always invalidates the service-config cache timer before return

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
- External/system dependencies: Notecard/Notehub transaction path through `NoteRequest`
- Data/type/global dependencies: directly writes static `serviceConfigTimer` in `n_helpers.c`; transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Creates a `hub.set` request. If request allocation succeeds, writes `"sn": "-"` for an empty serial number string, otherwise writes `"sn"` from `sn`, then returns the result of `NoteRequest(req)`. If request allocation fails, the request path leaves `success` false. On every return path, resets the static `serviceConfigTimer` to zero so cached service configuration will be refreshed.

## Important Boundaries

- Preconditions and assumptions: `sn` must be non-null when request allocation succeeds because the function reads `sn[0]` before any guarded helper call.
- Error/null/empty behavior: request allocation failure returns false but still clears `serviceConfigTimer`. Empty serial number is encoded as `"-"`. `JAddStringToObject` return values are ignored, so string allocation/attach failure is not surfaced before `NoteRequest`.
- Ownership and lifetime: serial strings are copied by the finalized JSON string helper; request ownership transfers to `NoteRequest`, which handles request/response cleanup through its finalized path.
- Concurrency/global state: directly mutates the static service-config cache timer without local locking; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: no platform-specific behavior in this wrapper; runtime effects go through portable hook layers.
- Performance shape: one request allocation, one JSON string add, one request transaction when allocation succeeds, and one static cache-timer write.
- Security or parsing constraints: does not validate serial-number format or verify the `"sn"` field was attached.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:2002`, the definition in `n_helpers.c:1963`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that set the Notecard serial number and force service-config cache refresh on subsequent reads.

## Tests Observed

Focused tests in `test/src/NoteSetSerialNumber_test.cpp` fake `NoteNewRequest` and `NoteRequest`. Covered cases: request allocation failure, `NoteRequest` false/true propagation, and empty string mapping to `"-"`. Gaps include null `sn`, non-empty serial field assertion, request name, JSON field-add failure, direct `serviceConfigTimer` invalidation assertion, and end-to-end transport behavior. The empty-string test comment says product but asserts `"sn"`.

## Graph Links

- Graph node id: `function:n_helpers.c:1963:NoteSetSerialNumber`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddStringToObject`, and `NoteRequest`.

## Evidence Gaps

No unresolved source gaps for this node.
