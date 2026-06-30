# `NoteDelayMs`

## Status

Finalized. Source-inspected delay hook dispatcher.

## Identity

- Kind: function
- Definition: `n_hooks.c:528`
- Declaration/interface: public note-c hook API declaration
- Signature/API shape: `void NoteDelayMs(uint32_t ms)`
- Visibility: public note-c hook API
- Public/exported names: `NoteDelayMs`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `hookDelayMs`

## Behavior

Calls `hookDelayMs(ms)` when a delay hook is registered; otherwise does nothing.

## Important Boundaries

- Preconditions and assumptions: delay value is forwarded unchanged.
- Error/null/empty behavior: null hook is a no-op.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads global hook pointer.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: platform sleep/delay behavior remains behind hook.

## Direct Call Sites

Exact source search found macro alias `_DelayMs` and public helper callers.

## Role In Callers

`_noteTransactionShouldLock` uses this through `_DelayMs` between retry attempts.

## Tests Observed

`NoteDelayMs_test.cpp` covers set/unset hook behavior; transaction tests cover retry delay calls.

## Graph Links

- Graph node id: `function:n_hooks.c:528:NoteDelayMs`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
