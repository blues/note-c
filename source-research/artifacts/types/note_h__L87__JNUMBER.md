# `JNUMBER`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `note.h:87`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `JNUMBER` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: default branch `typedef double JNUMBER`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Aliases a standard integer/floating type or another SDK typedef.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines JSON numeric storage as double on default platforms.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used across numeric parse/print/helper APIs and `J` struct `valuenumber` field.

## Role In Callers

Provides the public scalar contract for JSON numeric or time APIs.

## Tests Observed

Focused numeric parsing, printing, ftoa, and helper tests exercise active numeric behavior.

## Graph Links

- Graph node id: `typedef:note.h:87:JNUMBER`
- Structured graph: `inventory/dependency-graph.json`
- Representative `uses_type` graph edges connect consuming APIs to this typedef.

## Evidence Gaps

No unresolved source gaps for this node.
