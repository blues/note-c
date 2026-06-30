# `NoteSetLogLevel`

## Status

Finalized. Source-inspected public runtime log-level setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:444`
- Declaration/interface: `note.h:1042`
- Public/exported names: `NoteSetLogLevel`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetLogLevel(int level)`
- Visibility: public note-c hook/logging API implemented in `n_hooks.c`
- Ownership/lifetime role: updates module-local runtime log-level state in debug-enabled builds

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes `noteLogLevel` unless `NOTE_NODEBUG` is defined
- Artifact coverage: source-inspected artifact

## Behavior

When `NOTE_NODEBUG` is not defined, assigns `noteLogLevel = level`. In `NOTE_NODEBUG` builds, the function casts `level` to void and otherwise does nothing.

## Important Boundaries

- Preconditions and assumptions: accepts an integer level and does not validate it against known `NOTE_C_LOG_LEVEL_*` constants.
- Error/null/empty behavior: no return value or error path.
- Ownership and lifetime: writes only module-local logging state.
- Concurrency/global state: no locking around `noteLogLevel`.
- Portability/platform constraints: behavior is compile-time dependent; `NOTE_NODEBUG` makes the setter a no-op.
- Performance shape: one scalar assignment in debug-enabled builds.
- Security or parsing constraints: no range validation.

## Direct Call Sites

Source/header exact search found the public declaration in `note.h:1042`, source comments in headers, the definition in `n_hooks.c:444`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public API for downstream callers that want to adjust runtime filtering for SDK debug/log output in debug-enabled builds.

## Tests Observed

Focused tests in `test/src/NoteSetLogLevel_test.cpp` cover runtime setter behavior, and debug tests provide filtering evidence. Gaps include direct end-to-end assertions that `NoteSetLogLevel` changes `NoteDebugWithLevel` filtering behavior and invalid/range values.

## Graph Links

- Graph node id: `function:n_hooks.c:444:NoteSetLogLevel`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
