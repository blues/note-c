# `JUINTEGER`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `note.h:100`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `JUINTEGER` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: `typedef uint64_t JUINTEGER`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Aliases a standard integer/floating type or another SDK typedef.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines unsigned integer helper type, including safe conversion support for `JItoA`.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used by `JTIME` and `JItoA` unsigned conversion of negative values.

## Role In Callers

Provides the public scalar contract for JSON numeric or time APIs.

## Tests Observed

Focused `JItoA_test.cpp` covers min/max conversion behavior.

## Graph Links

- Graph node id: `typedef:note.h:100:JUINTEGER`
- Structured graph: `inventory/dependency-graph.json`
- Representative `uses_type` graph edges connect consuming APIs to this typedef.

## Evidence Gaps

No unresolved source gaps for this node.
