# `NoteSetContact`

## Status

Finalized. Source-inspected public contact write helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:2276`
- Declaration/interface: `note.h:2099`
- Public/exported names: `NoteSetContact`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteSetContact(char *nameBuf, char *orgBuf, char *roleBuf, char *emailBuf)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: reads caller-owned input strings, copies non-null values into a request, and transfers the request to `NoteRequest`

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
- External/system dependencies: Notecard transaction path through `NoteRequest`
- Data/type/global dependencies: transitive request/transaction globals and hooks through request callees
- Artifact coverage: source-inspected artifact

## Behavior

Creates a `card.contact` request. If request allocation fails, returns false. For each non-null input pointer, adds the corresponding string field: `"name"`, `"org"`, `"role"`, and `"email"`. Null input pointers omit their fields. Returns the `NoteRequest(req)` result.

## Important Boundaries

- Preconditions and assumptions: input pointers may be null. Non-null inputs are expected to point to valid NUL-terminated strings readable by `JAddStringToObject`.
- Error/null/empty behavior: request allocation failure returns false. All-null inputs still send a bare `card.contact` request. `JAddStringToObject` return values are ignored, so per-field allocation/attach failures can produce a partial request that is still sent.
- Ownership and lifetime: input string buffers remain caller-owned and are copied by `JAddStringToObject`; request ownership transfers to `NoteRequest`, which handles request/response cleanup through its finalized path.
- Concurrency/global state: no direct global state; transitive request path uses transaction state, locks, transport hooks, allocation hooks, and timing hooks.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: one request allocation, zero to four JSON string additions, and one request transaction.
- Security or parsing constraints: no local validation of contact field content or email format.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:2099`, the definition in `n_helpers.c:2276`, and focused tests. No production repository caller or alias was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that set the Notecard contact fields.

## Tests Observed

Focused tests in `test/src/NoteSetContact_test.cpp` fake `NoteNewRequest` and `NoteRequest`. Covered cases: request allocation failure, `NoteRequest` failure, all-null inputs returning true when the request succeeds, and all-valid inputs returning true. Gaps include request name, field shape assertions, mixed null/non-null inputs, JSON field-add failures, request ownership, response error handling, and real transaction hook behavior.

## Graph Links

- Graph node id: `function:n_helpers.c:2276:NoteSetContact`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddStringToObject`, and `NoteRequest`.

## Evidence Gaps

No unresolved source gaps for this node.
