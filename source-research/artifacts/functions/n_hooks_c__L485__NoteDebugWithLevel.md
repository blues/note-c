# `NoteDebugWithLevel`

## Status

Finalized. Source-inspected log-level filtered debug output helper.

## Identity

- Kind: function
- Definition: `n_hooks.c:485`
- Declaration/interface: `note.h:964`
- Signature/API shape: `void NoteDebugWithLevel(uint8_t level, const char *msg)`
- Visibility: public note-c debug API
- Public/exported names: `NoteDebugWithLevel`

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:256:_Debug` finalized
- External/system dependencies: none
- Data/type/global dependencies: `noteLogLevel`

## Behavior

When debugging is enabled, returns without output if `level > noteLogLevel`; otherwise emits `msg` through `_Debug`. Under `NOTE_NODEBUG`, suppresses output and marks arguments unused.

## Important Boundaries

- Preconditions and assumptions: message pointer is passed through to debug output when level permits.
- Error/null/empty behavior: levels above the runtime threshold are dropped.
- Ownership and lifetime: no allocation; passes borrowed message pointer.
- Concurrency/global state: reads global log level and debug hook state through `_Debug`.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: body is suppressed under `NOTE_NODEBUG`.

## Direct Call Sites

Exact source search found logging macros, `_errDoc`, and `NoteDebugWithLevelLn` as callers.

## Role In Callers

`NOTE_C_LOG_*` macros use this function to emit level prefixes and messages.

## Tests Observed

`NoteDebug_test.cpp` covers level filtering and output behavior.

## Graph Links

- Graph node id: `function:n_hooks.c:485:NoteDebugWithLevel`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
