# `_TransactionStop`

## Status

Finalized. Source-inspected transaction-stop macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:229`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_TransactionStop`
- Visibility: internal macro
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:694:_noteTransactionStop` finalized
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Expands directly to `_noteTransactionStop`.

## Important Boundaries

- Preconditions and assumptions: target hook is optional.
- Error/null/empty behavior: null hook is a no-op in the target.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows target hook state.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: platform transaction-end behavior remains behind hook target.

## Direct Call Sites

Exact source search found request, raw JSON, and ping paths call `_TransactionStop`.

## Role In Callers

`_noteTransactionShouldLock` calls this alias after a transaction has successfully started and is exiting.

## Tests Observed

Transaction hook tests cover stop behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:229:_TransactionStop`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
