# `typedef_82`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `n_cjson.c:82`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `typedef_82` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: anonymous `error` struct typedef with `json` pointer and `position` offset
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Internal cJSON parse-error state.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the translation-unit-local parse error record used by `global_error` and returned through `JGetErrorPtr`.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used by `global_error`, `JGetErrorPtr`, and `JParseWithOpts` parse error recording.

## Role In Callers

Stores parse error location without allocating.

## Tests Observed

Focused parse-error tests exercise `JGetErrorPtr` behavior.

## Graph Links

- Graph node id: `typedef:n_cjson.c:82:typedef_82`
- Structured graph: `inventory/dependency-graph.json`


## Evidence Gaps

No unresolved source gaps for this node.
