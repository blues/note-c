# `typedef_340`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `n_cjson.c:340`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `typedef_340` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: anonymous `printbuffer` struct typedef
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Uses `Jbool` fields for print options.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Carries output buffer pointer, capacity, offset, nesting depth, allocation mode, formatting flag, and omit-empty flag through cJSON print helpers.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used by `_ensure`, `_update_offset`, `_print`, `JPrintBuffered`, `JPrintPreallocated`, `_print_value`, `_print_array`, `_print_object`, and related print helpers.

## Role In Callers

Centralizes print buffer growth and formatted/unformatted/omit-empty behavior.

## Tests Observed

Focused cJSON print tests cover formatted, unformatted, preallocated, and omit-empty output behavior.

## Graph Links

- Graph node id: `typedef:n_cjson.c:340:typedef_340`
- Structured graph: `inventory/dependency-graph.json`


## Evidence Gaps

No unresolved source gaps for this node.
