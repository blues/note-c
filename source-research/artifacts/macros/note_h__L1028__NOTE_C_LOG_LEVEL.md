# `NOTE_C_LOG_LEVEL`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:1028`
- Declaration/interface: public header compile-time log level macro in `note.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: expands to `NOTE_C_LOG_LEVEL_DEFAULT` when not already defined.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: public header consumers and `n_hooks.c`.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: Guarded macro fallback dependency on `NOTE_C_LOG_LEVEL_DEFAULT`.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Provides the compile-time maximum log level used to initialize `noteLogLevel` in `n_hooks.c` when debug is enabled.

## Important Boundaries

- Compile-time override point via `#ifndef NOTE_C_LOG_LEVEL`.
- Runtime filtering compares message level against `noteLogLevel`; `NoteSetLogLevel` can change it after initialization.
- `NOTE_NODEBUG` compiles log operations to no-op behavior.

## Direct Call Sites

Used by `n_hooks.c:186` to initialize the static `noteLogLevel` global; `NoteDebugWithLevel` and `NoteDebugWithLevelLn` use that global for filtering.

## Role In Callers

Connects compile-time log configuration to runtime debug filtering.

## Tests Observed

Focused `NoteDebug_test.cpp` and `NoteSetLogLevel_test.cpp` cover default filtering and runtime level changes.

## Graph Links

- Graph node id: `macro:note.h:1028:NOTE_C_LOG_LEVEL`
- Structured graph: `inventory/dependency-graph.json`
- Verified `expands_to` edge to `NOTE_C_LOG_LEVEL_DEFAULT`.

## Evidence Gaps

No unresolved source gaps for this node.
