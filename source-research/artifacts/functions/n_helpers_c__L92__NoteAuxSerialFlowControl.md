# `NoteAuxSerialFlowControl`

## Status

Finalized. Source-inspected public AUX serial flow-control request helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:92`
- Declaration/interface: `note.h:1596`
- Public/exported names: `NoteAuxSerialFlowControl`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteAuxSerialFlowControl(int bufSize, int delayMs)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: creates a `card.aux.serial` request, transfers request ownership to `NoteRequest`, and returns the request success boolean

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:1989:JAddIntToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
- External/system dependencies: none
- Data/type/global dependencies: constant `c_mem`; transitive request/transaction and logging hooks through callees
- Artifact coverage: source-inspected artifact

## Behavior

Initializes `result` to false, creates a `card.aux.serial` request, and when allocation succeeds adds `"max": bufSize - 1` and `"ms": delayMs` before submitting the request through `NoteRequest`. The return value is exactly the bool returned by `NoteRequest`. If request allocation fails, it logs `ERRSTR("Failed to configure AUX serial flow control", c_mem)` and returns false.

## Important Boundaries

- Preconditions and assumptions: integer inputs are accepted as-is.
- Error/null/empty behavior: request allocation failure returns false and logs. `JAddIntToObject` failures are ignored; `NoteRequest` decides final success after whatever fields were inserted.
- Ownership and lifetime: a non-null request is consumed by `NoteRequest`; this helper does not delete it directly.
- Concurrency/global state: no direct local shared state; request submission and logging use global SDK hook layers through finalized callees.
- Portability/platform constraints: the helper is platform-neutral; I/O is behind `NoteRequest` and configured transaction hooks.
- Performance shape: one JSON request allocation, two best-effort integer insertions, one request transaction.
- Security or parsing constraints: no range validation; `bufSize - 1` can underflow for `INT_MIN` or produce negative `"max"` values for small inputs.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1596`, the definition in `n_helpers.c:92`, and focused tests in `test/src/NoteAuxSerialFlowControl_test.cpp`. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that configure the Notecard AUX serial flow-control buffer and read delay.

## Tests Observed

Focused tests in `test/src/NoteAuxSerialFlowControl_test.cpp` fake `NoteNewRequest`, `NoteRequest`, and logging. Covered cases include request object creation, request type `card.aux.serial`, `"max" == bufSize - 1`, `"ms" == delayMs`, successful true return, allocation-failure logging, and false return on allocation failure. Gaps include `NoteRequest` returning false, invalid numeric inputs, and JSON field insertion failure.

## Graph Links

- Graph node id: `function:n_helpers.c:92:NoteAuxSerialFlowControl`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddIntToObject`, `NoteRequest`, `NOTE_C_LOG_ERROR`, and `ERRSTR`.

## Evidence Gaps

No unresolved source gaps for this node.
