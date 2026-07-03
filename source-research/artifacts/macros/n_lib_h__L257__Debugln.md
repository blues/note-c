# `_Debugln`

## Status

Final. Macro definitions, public target, direct call sites, and focused debug tests have been inspected.

## Identity

- Kind: macro
- Definition: `n_lib.h:257`
- Declaration/interface: public debug function `NoteDebugln` is declared at `note.h:936`
- Public/exported names: internal macro `_Debugln`
- Macro aliases/wrappers: expands to nothing under `NOTE_NODEBUG`; otherwise expands to `NoteDebugln(x)`
- Signature/API shape: `_Debugln(x)` expression-like macro with one message argument
- Visibility: internal helper macro in `n_lib.h`
- Ownership/lifetime role: forwards/omits debug output; no ownership

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:464:NoteDebugln` through macro expansion when `NOTE_NODEBUG` is not defined (source target declared in `note.h:936`)
- External/system dependencies: configured debug output hook behind `NoteDebugln`
- Data/type/global dependencies: depends on compile-time `NOTE_NODEBUG`
- Artifact coverage: generated artifact exists

## Behavior

Internal debug-line macro. With `NOTE_NODEBUG` defined, `_Debugln(x)` expands to an empty statement-like macro and produces no output. Otherwise it forwards to `NoteDebugln(x)`, which appends a newline through the configured debug output path.

## Important Boundaries

- Preconditions and assumptions: caller supplies a valid debug string when debug is enabled
- Error/null/empty behavior: macro itself performs no validation
- Ownership and lifetime: does not allocate or retain the message
- Concurrency/global state: output behavior is determined by global debug hook/log configuration in `NoteDebugln`
- Portability/platform constraints: compile-time no-debug builds erase this call
- Performance shape: zero runtime cost under `NOTE_NODEBUG`; otherwise a direct debug call
- Security or parsing constraints: no parsing; debug output can expose caller-provided strings

## Direct Call Sites

Verified direct production caller: `n_helpers.c:2372` in `NoteDebugSyncStatus`.

## Role In Callers

Terminates sync-status debug output with the note text and newline.

## Tests Observed

`test/src/NoteDebug_test.cpp` covers `NoteDebugln("test")`, expecting `test\r\n` when debug is enabled and no output under `NOTE_NODEBUG`.

## Graph Links

- Graph node id: `macro:n_lib.h:257:_Debugln`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

The macro expansion target is tested through `NoteDebugln`; `_Debugln` itself is not separately faked in focused tests.
