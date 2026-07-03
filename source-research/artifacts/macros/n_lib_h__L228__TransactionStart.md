# `_TransactionStart`

## Status

Finalized. Source-inspected transaction-start macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:228`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_TransactionStart`
- Visibility: internal macro
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:681:_noteTransactionStart` finalized
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Expands directly to `_noteTransactionStart`.

## Important Boundaries

- Preconditions and assumptions: timeout argument is forwarded to the target.
- Error/null/empty behavior: target returns true when no hook is set.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows target hook state.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: platform readiness remains behind hook target.

## Direct Call Sites

Exact source search found request, raw JSON, and ping transaction paths call `_TransactionStart`.

## Role In Callers

`_noteTransactionShouldLock` aborts before I/O when this alias returns false.

## Tests Observed

Transaction hook and start-failure tests cover this alias.

## Graph Links

- Graph node id: `macro:n_lib.h:228:_TransactionStart`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
