# `NoteNewRequest`

## Status

Finalized. Source-inspected public request JSON constructor.

## Identity

- Kind: function
- Definition: `n_request.c:173`
- Declaration/interface: `note.h:336`
- Signature/API shape: `J *NoteNewRequest(const char *request)`
- Visibility: public SDK function declared in `note.h`
- Public/exported names: `NoteNewRequest`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2450:JCreateObject` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads global string constant `c_req` declared in `n_lib.h` and defined as `"req"` in `n_const.c`

## Behavior

Creates a JSON object with `JCreateObject`. If object allocation succeeds, adds a string field named by `c_req` (`"req"`) with value `request` via `JAddStringToObject`. Returns the request object pointer whether or not field attachment succeeds; returns `NULL` only when `JCreateObject` returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: normal callers pass a non-null Notecard request name; null request values cause `JAddStringToObject` to return `NULL` while `NoteNewRequest` still returns the empty object.
- Error/null/empty behavior: allocation failure returns `NULL`; field-add failure is not separately reported or cleaned up.
- Ownership and lifetime: caller owns the returned request object and must eventually delete it with `JDelete` or pass it to a request API that takes ownership.
- Concurrency/global state: no mutable global state.
- Performance shape: O(length of request string).

## Direct Call Sites

Exact source search found broad production fanout in high-level helper APIs (`n_helpers.c`), request orchestration helpers/tests in `n_request.c`, and integration/HITL code. In the active receive path, `NoteBinaryStoreReceive` calls `NoteNewRequest("card.binary.get")` before adding offset/length fields and sending the request through `_noteTransactionShouldLock`.

## Role In Callers

`NoteBinaryStoreReceive` treats a null return as request allocation failure and aborts the binary receive initialization path. Other helpers use the returned object as the base request document for Notecard APIs.

## Tests Observed

Focused `test/src/NoteNewRequest_test.cpp` verifies `NULL` is returned when `JCreateObject` fails and a non-null request object is returned when object allocation succeeds. Many helper tests fake `NoteNewRequest` to exercise allocation-failure behavior in callers.

## Graph Links

- Graph node id: `function:n_request.c:173:NoteNewRequest`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JCreateObject` and `JAddStringToObject`.

## Evidence Gaps

No unresolved source gaps for this node.
