# `NoteResetRequired`

## Status

Finalized. Source-inspected reset-required flag setter.

## Identity

- Kind: function
- Definition: `n_request.c:756`
- Declaration/interface: public note-c request API declaration in headers
- Signature/API shape: `void NoteResetRequired(void)`
- Visibility: public note-c API
- Public/exported names: `NoteResetRequired`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `resetRequired`

## Behavior

Sets the internal `resetRequired` flag to true.

## Important Boundaries

- Preconditions and assumptions: no inputs.
- Error/null/empty behavior: no error channel.
- Ownership and lifetime: no allocation.
- Concurrency/global state: mutates shared transaction/reset state.
- Performance shape: O(1).
- Portability/platform constraints: none.

## Direct Call Sites

Exact source search found callers in `_noteTransactionShouldLock` final error handling and heartbeat-abandon path, plus binary helper error paths.

## Role In Callers

The next transaction consumes this flag before I/O by attempting `_Reset`.

## Tests Observed

`NoteTransaction_test.cpp` and binary store tests exercise reset-required behavior indirectly.

## Graph Links

- Graph node id: `function:n_request.c:756:NoteResetRequired`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
