# `Jbool`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `n_cjson.h:103`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `Jbool` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: `typedef int Jbool`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Alias to `int`.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the cJSON-compatible boolean return/parameter type used by parse/print/classifier APIs.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used broadly by cJSON functions such as `JParseWithOpts`, `JCreateBool`, `JPrintPreallocated`, classifiers, compare, and internal parse/print helpers.

## Role In Callers

Provides ABI-compatible boolean-ish integer type distinct from C `bool` in the cJSON layer.

## Tests Observed

Focused cJSON tests exercise functions returning and accepting `Jbool`.

## Graph Links

- Graph node id: `typedef:n_cjson.h:103:Jbool`
- Structured graph: `inventory/dependency-graph.json`
- Representative `uses_type` edges connect cJSON bool APIs to this typedef.

## Evidence Gaps

No unresolved source gaps for this node.
