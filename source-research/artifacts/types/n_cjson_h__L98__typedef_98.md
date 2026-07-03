# `typedef_98`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `n_cjson.h:98`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `typedef_98` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: `typedef struct JHooks { malloc_fn, free_fn } JHooks`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Uses function pointer fields for allocation/free callbacks.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines custom allocator hooks for cJSON-compatible memory management. It carries allocator and free function pointers.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Declared in `n_cjson.h`; exact source search found the public `JInitHooks(JHooks *hooks)` declaration but no in-repo implementation body.

## Role In Callers

Preserves cJSON API compatibility for allocator customization.

## Tests Observed

No focused test found for `JInitHooks`/`JHooks` behavior.

## Graph Links

- Graph node id: `typedef:n_cjson.h:98:typedef_98`
- Structured graph: `inventory/dependency-graph.json`


## Evidence Gaps

No unresolved source gaps for this node.
