# `_DebugIntln`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:258`
- Declaration/interface: internal debug wrapper macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: expands to `NoteDebugIntln(x, y)` when debug is enabled; the `NOTE_NODEBUG` branch is a no-op.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: translation units including `n_lib.h`.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: Alias/wrapper dependency on `NoteDebugIntln` in the debug-enabled branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Provides a readability wrapper for integer debug-line logging. In normal debug-enabled builds it delegates to `NoteDebugIntln`; under `NOTE_NODEBUG` the alternate branch compiles away its arguments.

## Important Boundaries

- Compile-time debug branch.
- No production source call sites currently invoke `_DebugIntln`; callers use other debug wrappers or public `NoteDebugIntln`.
- The public function itself checks for null message before printing the number and newline.

## Direct Call Sites

Definition is in `n_lib.h:258`; exact source search found no production `_DebugIntln(...)` call sites.

## Role In Callers

Available internal shorthand for integer debug output without changing public debug semantics.

## Tests Observed

Focused `NoteDebug_test.cpp` covers the public `NoteDebugIntln` behavior for null and non-null message prefixes.

## Graph Links

- Graph node id: `macro:n_lib.h:258:_DebugIntln`
- Structured graph: `inventory/dependency-graph.json`
- Verified `expands_to` edge to `NoteDebugIntln`.

## Evidence Gaps

No unresolved source gaps for this node.
