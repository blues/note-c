# `typedef_78`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `n_cjson.h:78`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `typedef_78` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: `typedef struct J { ... } J`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Depends on `JINTEGER` and `JNUMBER` fields.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the core JSON object type used throughout note-c. The struct includes linked-list pointers, child pointer, type flags, string/number values, and object key string.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Referenced by nearly all JSON APIs as `J *` or `const J *`.

## Role In Callers

Provides the central in-memory JSON representation. Public comments advise treating it as opaque even though fields are visible for cJSON compatibility.

## Tests Observed

Focused cJSON and helper tests cover creation, parsing, printing, lookup, deletion, and mutation behavior.

## Graph Links

- Graph node id: `typedef:n_cjson.h:78:typedef_78`
- Structured graph: `inventory/dependency-graph.json`
- Representative `uses_type` edges connect core JSON functions to this typedef.

## Evidence Gaps

No unresolved source gaps for this node.
