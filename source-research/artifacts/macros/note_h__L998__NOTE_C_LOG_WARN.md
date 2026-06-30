# `NOTE_C_LOG_WARN`

## Status

Finalized. Source-inspected warn-level logging macro.

## Identity

- Kind: macro
- Definition: `note.h:998`
- Declaration/interface: public logging macro unless overridden
- Signature/API shape: `NOTE_C_LOG_WARN(msg)`
- Visibility: public logging macro
- Public/exported names: `NOTE_C_LOG_WARN`

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:485:NoteDebugWithLevel` finalized
  - `function:n_hooks.c:499:NoteDebugWithLevelLn` finalized
- External/system dependencies: none
- Data/type/global dependencies: optional `NOTE_C_LOG_FILE_AND_LINE`

## Behavior

Emits optional file/line prefix, then writes `[WARN] ` and the message plus newline at `NOTE_C_LOG_LEVEL_WARN`.

## Important Boundaries

- Preconditions and assumptions: message pointer is forwarded to debug output.
- Error/null/empty behavior: runtime log level may drop the output.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows debug hook and log-level state.
- Performance shape: bounded macro expansion plus debug hook calls.
- Portability/platform constraints: file/line prefix depends on `NOTE_C_LOG_SHOW_FILE_AND_LINE`.

## Direct Call Sites

Exact source search found transaction retry paths and helper warnings use this macro.

## Role In Callers

`_noteTransactionShouldLock` uses warning logs before retrying transaction, no-response, CRC, and corrupt-response paths.

## Tests Observed

`NoteDebug_test.cpp` and transaction retry tests exercise log output indirectly.

## Graph Links

- Graph node id: `macro:note.h:998:NOTE_C_LOG_WARN`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
