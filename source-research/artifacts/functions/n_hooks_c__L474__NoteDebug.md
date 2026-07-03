# `NoteDebug`

## Status

Finalized. Source-inspected debug-output hook dispatcher.

## Identity

- Kind: function
- Definition: `n_hooks.c:474`
- Declaration/interface: public note-c debug API declaration
- Signature/API shape: `void NoteDebug(const char *msg)`
- Visibility: public note-c debug API
- Public/exported names: `NoteDebug`

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:308:_noteIsDebugOutputActive` finalized
- External/system dependencies: none
- Data/type/global dependencies: `hookDebugOutput`

## Behavior

When `NOTE_NODEBUG` is not defined, calls `hookDebugOutput(msg)` only if debug output is active. Under `NOTE_NODEBUG`, suppresses output and marks the argument unused.

## Important Boundaries

- Preconditions and assumptions: active debug output implies a non-null hook via `_noteIsDebugOutputActive`.
- Error/null/empty behavior: inactive debug output is a no-op.
- Ownership and lifetime: no allocation; passes borrowed message pointer.
- Concurrency/global state: reads debug hook state.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: compiled out under `NOTE_NODEBUG`.

## Direct Call Sites

Exact source search found macro alias `_Debug`, debug helper wrappers, and logging paths as callers.

## Role In Callers

`NoteDebugWithLevel` reaches this function through `_Debug` after log-level filtering.

## Tests Observed

`NoteDebug_test.cpp` covers debug output behavior.

## Graph Links

- Graph node id: `function:n_hooks.c:474:NoteDebug`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
