# `_DebugWithLevelLn`

## Status

Finalized. Source-inspected line-terminated log-level debug macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:260`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_DebugWithLevelLn(x, y)`
- Visibility: internal macro
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:499:NoteDebugWithLevelLn` finalized when debugging is enabled
- External/system dependencies: none
- Data/type/global dependencies: `NOTE_NODEBUG`

## Behavior

Expands to nothing under `NOTE_NODEBUG`; otherwise expands to `NoteDebugWithLevelLn(x, y)`.

## Important Boundaries

- Preconditions and assumptions: macro argument behavior depends on compile mode.
- Error/null/empty behavior: nodebug builds suppress output completely.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows debug target state.
- Performance shape: O(1) plus hook cost when enabled.
- Portability/platform constraints: compile-time debug mode changes expansion.

## Direct Call Sites

Exact source search found `_errDoc` and logging macros use this alias.

## Role In Callers

`_errDoc` uses this alias to terminate its manually assembled error log line.

## Tests Observed

`NoteDebug_test.cpp` and `_errDoc_test.cpp` cover line output behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:260:_DebugWithLevelLn`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
