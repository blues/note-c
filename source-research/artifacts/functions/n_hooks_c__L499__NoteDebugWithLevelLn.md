# `NoteDebugWithLevelLn`

## Status

Finalized. Source-inspected log-level filtered debug-line helper.

## Identity

- Kind: function
- Definition: `n_hooks.c:499`
- Declaration/interface: `note.h:976`
- Signature/API shape: `void NoteDebugWithLevelLn(uint8_t level, const char *msg)`
- Visibility: public note-c debug API
- Public/exported names: `NoteDebugWithLevelLn`

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:259:_DebugWithLevel` finalized
- External/system dependencies: none
- Data/type/global dependencies: `noteLogLevel`, `c_newline`

## Behavior

When debugging is enabled, returns without output if `level > noteLogLevel`; otherwise emits `msg` and then `c_newline` through `_DebugWithLevel`. Under `NOTE_NODEBUG`, suppresses output and marks arguments unused.

## Important Boundaries

- Preconditions and assumptions: message pointer is passed through to debug output when level permits.
- Error/null/empty behavior: levels above threshold are dropped.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads global log level and debug hook state through `_DebugWithLevel`.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: body is suppressed under `NOTE_NODEBUG`.

## Direct Call Sites

Exact source search found `NOTE_C_LOG_*` macros and `_errDoc` debug output as callers.

## Role In Callers

Logging macros use this function for line-terminated message output.

## Tests Observed

`NoteDebug_test.cpp` covers level filtering and line output behavior.

## Graph Links

- Graph node id: `function:n_hooks.c:499:NoteDebugWithLevelLn`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
