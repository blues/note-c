# `NOTE_C_LOG_LEVEL_DEFAULT`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:1022`
- Declaration/interface: public header compile-time log level default macro in `note.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: expands to `NOTE_C_LOG_LEVEL_INFO`.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: public header consumers and `n_hooks.c`.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: Macro alias dependency on `NOTE_C_LOG_LEVEL_INFO`.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the default maximum log level used when `NOTE_C_LOG_LEVEL` is not supplied at compile time.

## Important Boundaries

- Compile-time default.
- Can be overridden by defining `NOTE_C_LOG_LEVEL` before the guarded definition.
- Runtime log level can still be changed through `NoteSetLogLevel` when debug is enabled.

## Direct Call Sites

Used by the guarded `NOTE_C_LOG_LEVEL` definition in `note.h:1028`; focused tests also reset `noteLogLevel` to this macro.

## Role In Callers

Makes info-level logging the default ceiling for debug-level filtering.

## Tests Observed

Focused `NoteSetLogLevel_test.cpp` uses `NOTE_C_LOG_LEVEL_DEFAULT` and verifies runtime override behavior.

## Graph Links

- Graph node id: `macro:note.h:1022:NOTE_C_LOG_LEVEL_DEFAULT`
- Structured graph: `inventory/dependency-graph.json`
- Verified `expands_to` edge to `NOTE_C_LOG_LEVEL_INFO`.

## Evidence Gaps

No unresolved source gaps for this node.
