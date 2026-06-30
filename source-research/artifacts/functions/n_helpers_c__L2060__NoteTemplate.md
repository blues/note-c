# `NoteTemplate`

## Status

Finalized. Source-inspected public Notefile template helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:2060`
- Declaration/interface: `note.h:2209`
- Public/exported names: `NoteTemplate`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteTemplate(const char *target, J *body)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: consumes/frees `body` on all visible paths by deleting it when request allocation fails or attaching it into the request before `NoteRequest`

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_cjson.c:1866:JAddItemToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
- External/system dependencies: Notecard transaction path through `NoteRequest`
- Data/type/global dependencies: JSON object ownership and transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Creates a `note.template` request. If request allocation fails, deletes `body` and returns false. Otherwise adds `"file"` from `target`, attaches `"body"` by transferring `body` into the request, and returns `NoteRequest(req)`.

## Important Boundaries

- Preconditions and assumptions: `target` and `body` are not locally validated. Null `target` is delegated to `JAddStringToObject`, whose null return is ignored. Null `body` is delegated to `JAddItemToObject`, which handles null item behavior through its finalized path.
- Error/null/empty behavior: request allocation failure deletes `body` and returns false. Field-add failures for `"file"` or `"body"` are ignored before `NoteRequest`; success is determined by `NoteRequest`.
- Ownership and lifetime: caller gives up ownership of `body`. On request allocation failure `JDelete(body)` frees it; on request allocation success `JAddItemToObject(req, "body", body)` transfers ownership to `req`, and `NoteRequest` consumes/deletes the request through its finalized path.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: one request allocation, one JSON string add, one JSON object attach, and one request transaction.
- Security or parsing constraints: does not validate notefile target or template body shape locally.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:2209`, the definition in `n_helpers.c:2060`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that set a Notefile template and transfer ownership of the template body to the SDK request path.

## Tests Observed

Focused tests in `test/src/NoteTemplate_test.cpp` fake `NoteNewRequest` and `NoteRequest`. Covered cases: request allocation failure returns false, `NoteRequest` false propagates, and `NoteRequest` true propagates. Gaps include request name, `"file"` and `"body"` request shape, null `target`, null `body`, add-helper allocation failures, and direct ownership/free assertions.

## Graph Links

- Graph node id: `function:n_helpers.c:2060:NoteTemplate`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JDelete`, `JAddStringToObject`, `JAddItemToObject`, and `NoteRequest`.

## Evidence Gaps

No unresolved source gaps for this node.
