# `NOTE_C_LOG_LEVEL_DEBUG`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:956`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_C_LOG_LEVEL_DEBUG`
- Macro aliases/wrappers: literal `3`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, field access, or wrapper expression.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines an ordered debug/log severity level. `NoteDebugWithLevel` and `NoteDebugWithLevelLn` return early when the message level is greater than runtime `noteLogLevel`. Used by `NOTE_C_LOG_DEBUG`.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers can use this value/expression wherever `note.h` is included.
- Exact source search distinguishes macro use from related literal or function use.

## Direct Call Sites

Used in logging wrapper macros in `note.h:990-1017`; `NOTE_C_LOG_LEVEL_ERROR` is also used directly in `_errDoc` and `_noteTransactionShouldLock` error logging.

## Role In Callers

Provides stable numeric severity ordering for compile-time logging macros and runtime filtering.

## Tests Observed

Focused debug tests cover default DEBUG suppression.

## Graph Links

- Graph node id: `macro:note.h:956:NOTE_C_LOG_LEVEL_DEBUG`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges from logging wrappers and direct users where present.

## Evidence Gaps

No unresolved source gaps for this node.
