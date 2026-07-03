# `NOTE_C_LOG_INFO`

## Status

Finalized. Source-inspected info-level logging macro.

## Identity

- Kind: macro
- Definition: `note.h:1006`
- Declaration/interface: public logging macro unless overridden
- Signature/API shape: `NOTE_C_LOG_INFO(msg)`
- Visibility: public logging macro
- Public/exported names: `NOTE_C_LOG_INFO`

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:485:NoteDebugWithLevel` finalized
  - `function:n_hooks.c:499:NoteDebugWithLevelLn` finalized
- External/system dependencies: none
- Data/type/global dependencies: optional `NOTE_C_LOG_FILE_AND_LINE`

## Behavior

Emits optional file/line prefix, then writes `[INFO] ` and the message plus newline at `NOTE_C_LOG_LEVEL_INFO`.

## Important Boundaries

- Preconditions and assumptions: message pointer is forwarded to debug output.
- Error/null/empty behavior: runtime log level may drop the output.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows debug hook and log-level state.
- Performance shape: bounded macro expansion plus debug hook calls.
- Portability/platform constraints: file/line prefix depends on `NOTE_C_LOG_SHOW_FILE_AND_LINE`.

## Direct Call Sites

Exact source search found request and response transaction trace logging uses this macro.

## Role In Callers

`_noteTransactionShouldLock` uses info logs to trace outgoing request JSON and incoming response JSON when transaction display is not suppressed.

## Tests Observed

`NoteDebug_test.cpp` and transaction logging tests exercise log output indirectly.

## Graph Links

- Graph node id: `macro:note.h:1006:NOTE_C_LOG_INFO`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
