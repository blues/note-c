# `JNTOA_PRECISION`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:1443`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `JNTOA_PRECISION`
- Macro aliases/wrappers: literal `(16)`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, field access, or wrapper expression.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Sets the default precision used by `JNtoA` when callers pass a negative precision.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers can use this value/expression wherever `note.h` is included.
- Exact source search distinguishes macro use from related literal or function use.

## Direct Call Sites

Used by `JNtoA` in `n_ftoa.c:62`; exact search found no other production uses.

## Role In Callers

Defines the SDK default number-to-string precision for JSON and helper formatting.

## Tests Observed

Focused `JNtoA_test.cpp` exercises negative precision and conversion behavior.

## Graph Links

- Graph node id: `macro:note.h:1443:JNTOA_PRECISION`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `JNtoA`.

## Evidence Gaps

No unresolved source gaps for this node.
