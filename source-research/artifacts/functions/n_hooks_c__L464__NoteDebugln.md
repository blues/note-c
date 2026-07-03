# `NoteDebugln`

## Status

Final. Behavior, public declaration, macro dependency, direct call sites, compile-time behavior, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:464`
- Declaration/interface: `note.h:936`
- Public/exported names: `NoteDebugln`
- Macro aliases/wrappers: internal `_Debugln(x)` macro expands to this function when `NOTE_NODEBUG` is not defined
- Signature/API shape: `void NoteDebugln(const char *msg)`
- Visibility: public SDK debug API
- Ownership/lifetime role: emits caller-provided string and newline through debug output hook; does not retain data

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:256:_Debug` (calls twice when debug is enabled, verified)
- External/system dependencies: configured debug output hook behind `_Debug`/`NoteDebug`
- Data/type/global dependencies: compile-time `NOTE_NODEBUG`
- Artifact coverage: generated artifact exists

## Behavior

When `NOTE_NODEBUG` is not defined, calls `_Debug(msg)` and then `_Debug(c_newline)`, producing the message followed by CRLF through the configured debug output hook. When `NOTE_NODEBUG` is defined, casts `msg` to void and emits nothing.

## Important Boundaries

- Preconditions and assumptions: output requires a configured debug hook and a non-`NOTE_NODEBUG` build
- Error/null/empty behavior: no explicit null guard; `NULL` handling is delegated to `_Debug`/`NoteDebug`
- Ownership and lifetime: does not allocate or retain `msg`
- Concurrency/global state: depends on global debug hook state used by `NoteDebug`
- Portability/platform constraints: no-op in `NOTE_NODEBUG` builds
- Performance shape: two debug calls when enabled; zero output in no-debug builds
- Security or parsing constraints: no parsing; may emit caller-provided text

## Direct Call Sites

Verified direct production references: public declaration in `note.h:936`, definition in `n_hooks.c:464`, and `_Debugln` macro expansion in `n_lib.h:257`. Focused direct test caller is `test/src/NoteDebug_test.cpp:72`.

## Role In Callers

Public debug-line helper and target of the internal `_Debugln` macro.

## Tests Observed

`test/src/NoteDebug_test.cpp` verifies `NoteDebugln("test")` appends `\r\n` when debug is enabled and emits nothing under `NOTE_NODEBUG`.

## Graph Links

- Graph node id: `function:n_hooks.c:464:NoteDebugln`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers `NoteDebugln(NULL)`; source delegates that behavior to `_Debug`/`NoteDebug`.
