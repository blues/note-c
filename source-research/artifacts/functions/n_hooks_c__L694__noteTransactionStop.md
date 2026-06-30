# `_noteTransactionStop`

## Status

Finalized. Source-inspected transaction-stop hook dispatcher.

## Identity

- Kind: function
- Definition: `n_hooks.c:694`
- Declaration/interface: internal hook target
- Signature/API shape: `void _noteTransactionStop(void)`
- Visibility: internal note-c hook boundary
- Public/exported names: `_noteTransactionStop`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `hookTransactionStop`

## Behavior

Calls `hookTransactionStop()` when registered; otherwise does nothing.

## Important Boundaries

- Preconditions and assumptions: no inputs.
- Error/null/empty behavior: null hook is a no-op.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads global hook pointer.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: platform low-power/end-of-transaction behavior remains behind hook.

## Direct Call Sites

Exact source search found macro alias `_TransactionStop` as the primary internal caller surface.

## Role In Callers

Transaction paths call this after a successful `_TransactionStart` during normal completion and error exits.

## Tests Observed

`NoteTransactionHooks_test.cpp`, `NoteTransaction_test.cpp`, and raw JSON transaction tests cover stop-hook pairing.

## Graph Links

- Graph node id: `function:n_hooks.c:694:_noteTransactionStop`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
