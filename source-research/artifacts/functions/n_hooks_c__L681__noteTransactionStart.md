# `_noteTransactionStart`

## Status

Finalized. Source-inspected transaction-start hook dispatcher.

## Identity

- Kind: function
- Definition: `n_hooks.c:681`
- Declaration/interface: internal hook target
- Signature/API shape: `bool _noteTransactionStart(uint32_t timeoutMs)`
- Visibility: internal note-c hook boundary
- Public/exported names: `_noteTransactionStart`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `hookTransactionStart`

## Behavior

Calls `hookTransactionStart(timeoutMs)` when registered and returns its result. With no hook, returns true.

## Important Boundaries

- Preconditions and assumptions: timeout is forwarded unchanged.
- Error/null/empty behavior: null hook permits the transaction.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads global hook pointer.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: platform readiness behavior remains behind hook.

## Direct Call Sites

Exact source search found macro alias `_TransactionStart` as the primary internal caller surface.

## Role In Callers

`_noteTransactionShouldLock` aborts early when this hook returns false.

## Tests Observed

`NoteTransactionHooks_test.cpp` and `NoteTransaction_test.cpp` cover start-hook behavior and start failure.

## Graph Links

- Graph node id: `function:n_hooks.c:681:_noteTransactionStart`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
