# `_noteHardReset`

## Status

Finalized. Source-inspected active-interface reset dispatcher.

## Identity

- Kind: function
- Definition: `n_hooks.c:1015`
- Declaration/interface: internal hook target
- Signature/API shape: `bool _noteHardReset(void)`
- Visibility: internal note-c hook boundary
- Public/exported names: `_noteHardReset`

## Dependency Status

- Project-local dependencies: selected reset target is dynamic through `notecardReset`
- External/system dependencies: none
- Data/type/global dependencies: `notecardReset`

## Behavior

Returns true when no reset target is selected. Otherwise calls the active `notecardReset` function pointer and returns its result.

## Important Boundaries

- Preconditions and assumptions: active interface selection owns the function pointer.
- Error/null/empty behavior: no active reset target is treated as success.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads global dispatch pointer.
- Performance shape: O(1) plus active adapter reset cost.
- Portability/platform constraints: serial/I2C reset behavior remains in adapter targets selected by `_noteSetActiveInterface`.

## Direct Call Sites

Exact source search found macro alias `_Reset` and `NoteReset` as callers.

## Role In Callers

`_noteTransactionShouldLock` uses this through `_Reset` when `resetRequired` is set or I/O errors require retry reset.

## Tests Observed

`_noteHardReset_test.cpp`, `_noteSetActiveInterface_test.cpp`, and transaction reset tests cover default and selected reset behavior.

## Graph Links

- Graph node id: `function:n_hooks.c:1015:_noteHardReset`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
