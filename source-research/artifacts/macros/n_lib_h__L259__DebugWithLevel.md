# `_DebugWithLevel`

## Status

Finalized. Source-inspected log-level debug macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:259`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_DebugWithLevel(x, y)`
- Visibility: internal macro
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:485:NoteDebugWithLevel` finalized when debugging is enabled
- External/system dependencies: none
- Data/type/global dependencies: `NOTE_NODEBUG`

## Behavior

Expands to nothing under `NOTE_NODEBUG`; otherwise expands to `NoteDebugWithLevel(x, y)`.

## Important Boundaries

- Preconditions and assumptions: macro argument behavior depends on compile mode.
- Error/null/empty behavior: nodebug builds suppress output completely.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows debug target state.
- Performance shape: O(1) plus hook cost when enabled.
- Portability/platform constraints: compile-time debug mode changes expansion.

## Direct Call Sites

Exact source search found `_errDoc` and parse/transaction error reporting use this alias.

## Role In Callers

`_noteTransactionShouldLock` uses this alias to log invalid response JSON in non-low-memory builds.

## Tests Observed

`NoteDebug_test.cpp` and `_errDoc_test.cpp` cover debug output behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:259:_DebugWithLevel`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
