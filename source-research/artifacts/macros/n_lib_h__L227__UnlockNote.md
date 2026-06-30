# `_UnlockNote`

## Status

Finalized. Source-inspected Notecard unlock macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:227`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_UnlockNote`
- Visibility: internal macro
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:669:_noteUnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Expands directly to `_noteUnlockNote`.

## Important Boundaries

- Preconditions and assumptions: function-like calls through the alias use `_noteUnlockNote` semantics.
- Error/null/empty behavior: null hook is a no-op in the target.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows target hook state.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: platform behavior remains behind hook target.

## Direct Call Sites

Exact source search found transaction and reset paths call `_UnlockNote`.

## Role In Callers

`_noteTransactionShouldLock` releases the Notecard lock through this alias when `lockNotecard` is true.

## Tests Observed

Transaction hook tests cover unlock behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:227:_UnlockNote`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
