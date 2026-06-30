# `typedef_239`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `n_cjson.c:239`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `typedef_239` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: anonymous `parse_buffer` struct typedef
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Internal parser state object.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Carries parser content pointer, length, offset, and recursion depth through cJSON parse helpers.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used by `JParseWithOpts`, whitespace/BOM skip helpers, `_parse_value`, `_parse_array`, `_parse_object`, `_parse_number`, and `_parse_string`.

## Role In Callers

Centralizes parser cursor and nesting-depth state, including nesting-limit enforcement.

## Tests Observed

Focused cJSON parse tests cover parsing behavior; nesting-limit tests cover depth behavior.

## Graph Links

- Graph node id: `typedef:n_cjson.c:239:typedef_239`
- Structured graph: `inventory/dependency-graph.json`


## Evidence Gaps

No unresolved source gaps for this node.
