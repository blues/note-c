# `NOTE_C_LOG_DEBUG`

## Status

Finalized. Source-inspected normal debug-level logging macro.

## Identity

- Kind: macro
- Definition: `note.h:1014`
- Declaration/interface: public logging macro unless already defined
- Signature/API shape: `NOTE_C_LOG_DEBUG(msg)`
- Visibility: public logging macro
- Public/exported names: `NOTE_C_LOG_DEBUG`

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:485:NoteDebugWithLevel` finalized
  - `function:n_hooks.c:499:NoteDebugWithLevelLn` finalized
- External/system dependencies: none
- Data/type/global dependencies: optional `NOTE_C_LOG_FILE_AND_LINE`

## Behavior

Emits optional file/line prefix, then writes `[DEBUG] ` and the message plus newline at `NOTE_C_LOG_LEVEL_DEBUG`. This normal variant is not defined when the low-memory no-op variant has already defined `NOTE_C_LOG_DEBUG`.

## Important Boundaries

- Preconditions and assumptions: message pointer is forwarded to debug output.
- Error/null/empty behavior: default runtime log level drops debug output unless configured to DEBUG.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows debug hook and log-level state.
- Performance shape: bounded macro expansion plus debug hook calls.
- Portability/platform constraints: low-memory builds use the earlier no-op variant at `note.h:76`.

## Direct Call Sites

Exact source search found transaction, helper, and hook debug logging calls.

## Role In Callers

`_noteTransactionShouldLock` uses debug logs for user-agent insertion, reset, command success, fatal transaction failures, heartbeat status, and retry decisions.

## Tests Observed

`NoteDebug_test.cpp`, `_errDoc_test.cpp`, and transaction tests exercise debug logging behavior indirectly.

## Graph Links

- Graph node id: `macro:note.h:1014:NOTE_C_LOG_DEBUG`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
