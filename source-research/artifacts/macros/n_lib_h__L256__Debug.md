# `_Debug`

## Status

Finalized. Source-inspected debug macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:256`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_Debug(x)`
- Visibility: internal macro
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:474:NoteDebug` finalized when debugging is enabled
- External/system dependencies: none
- Data/type/global dependencies: `NOTE_NODEBUG`

## Behavior

Expands to nothing under `NOTE_NODEBUG`; otherwise expands to `NoteDebug(x)`.

## Important Boundaries

- Preconditions and assumptions: macro argument behavior depends on compile mode.
- Error/null/empty behavior: nodebug builds suppress output completely.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows `NoteDebug`.
- Performance shape: O(1) plus debug hook cost when enabled.
- Portability/platform constraints: compile-time debug mode changes expansion.

## Direct Call Sites

Exact source search found debug helper functions and debug/log paths use this macro.

## Role In Callers

`NoteDebugWithLevel` uses `_Debug` after runtime log-level filtering.

## Tests Observed

`NoteDebug_test.cpp` covers debug output behavior in enabled and disabled configurations.

## Graph Links

- Graph node id: `macro:n_lib.h:256:_Debug`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
