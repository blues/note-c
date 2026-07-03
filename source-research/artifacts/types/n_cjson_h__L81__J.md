# `J`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: struct field
- Definition: `n_cjson.h:81`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `J` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: `prev` sibling pointer inside `typedef struct J`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Field belongs to `typedef struct J` / `J`.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Represents a recursive pointer field in the public cJSON-compatible `J` object struct; it is not a separate project-defined struct despite the generated node kind.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used throughout cJSON object/array manipulation through `J *` objects; exact field use is concentrated in list traversal, deletion, duplication, and object/array helpers.

## Role In Callers

Maintains linked-list and child-chain topology for JSON arrays and objects.

## Tests Observed

Focused cJSON tests exercise object/array creation, traversal, deletion, and duplication behavior.

## Graph Links

- Graph node id: `struct:n_cjson.h:81:J`
- Structured graph: `inventory/dependency-graph.json`
- Linked to representative `J` object users in the graph.

## Evidence Gaps

No unresolved source gaps for this node.
