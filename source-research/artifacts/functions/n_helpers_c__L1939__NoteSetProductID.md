# `NoteSetProductID`

## Status

Finalized. Source-inspected public product-id setter helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1939`
- Declaration/interface: `note.h:2010`
- Public/exported names: `NoteSetProductID`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteSetProductID(const char *productID)`
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

Creates a `hub.set` request. If request allocation succeeds, writes `"product": "-"` for an empty `productID` string, otherwise writes `"product"` from `productID`, then returns the result of `NoteRequest(req)`. If request allocation fails, the request path leaves `success` false. On every return path, resets the static `serviceConfigTimer` to zero so cached service configuration will be refreshed.

## Important Boundaries

- Preconditions and assumptions: `productID` must be non-null when request allocation succeeds because the function reads `productID[0]` before any guarded helper call.
- Error/null/empty behavior: request allocation failure returns false but still clears `serviceConfigTimer`. Empty product ID is encoded as `"-"`. `JAddStringToObject` return values are ignored, so string allocation/attach failure is not surfaced before `NoteRequest`.
- Ownership and lifetime: product strings are copied by the finalized JSON string helper; request ownership transfers to `NoteRequest`, which handles request/response cleanup through its finalized path.
- Concurrency/global state: directly mutates the static service-config cache timer without local locking; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: no platform-specific behavior in this wrapper; runtime effects go through portable hook layers.
- Performance shape: one request allocation, one JSON string add, one request transaction when allocation succeeds, and one static cache-timer write.
- Security or parsing constraints: does not validate product UID format or verify the `"product"` field was attached.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:2010`, the definition in `n_helpers.c:1939`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that set the Notecard ProductUID and force service-config cache refresh on subsequent reads.

## Tests Observed

Focused tests in `test/src/NoteSetProductID_test.cpp` fake `NoteNewRequest` and `NoteRequest`. Covered cases: request allocation failure, `NoteRequest` false/true propagation, and empty string mapping to `"-"`. Gaps include null `productID`, non-empty product field assertion, JSON field-add failure, direct `serviceConfigTimer` invalidation assertion, and end-to-end transport/user-agent behavior.

## Graph Links

- Graph node id: `function:n_helpers.c:1939:NoteSetProductID`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddStringToObject`, and `NoteRequest`.

## Evidence Gaps

No unresolved source gaps for this node.
