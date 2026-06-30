# `_noteUnlockNote`

## Status

Finalized. Source-inspected Notecard unlock hook dispatcher.

## Identity

- Kind: function
- Definition: `n_hooks.c:669`
- Declaration/interface: internal hook target
- Signature/API shape: `void _noteUnlockNote(void)`
- Visibility: internal note-c hook boundary
- Public/exported names: `_noteUnlockNote`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `hookUnlockNote`

## Behavior

Calls `hookUnlockNote()` when a hook is registered; otherwise does nothing.

## Important Boundaries

- Preconditions and assumptions: no inputs.
- Error/null/empty behavior: null hook is a no-op.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads global hook pointer.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: platform-specific behavior remains behind hook.

## Direct Call Sites

Exact source search found macro alias `_UnlockNote` as the primary internal caller surface.

## Role In Callers

`_noteTransactionShouldLock` uses this through `_UnlockNote` only when it acquired the Notecard lock.

## Tests Observed

`NoteTransactionHooks_test.cpp` and transaction tests exercise lock/unlock hook behavior.

## Graph Links

- Graph node id: `function:n_hooks.c:669:_noteUnlockNote`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
