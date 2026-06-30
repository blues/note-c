# `typedef_1523`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `note.h:1523`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `typedef_1523` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: anonymous struct typedef `NoteMD5Context` with `buf[4]`, `bits[2]`, and `in[64]`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Public MD5 context struct.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the public incremental MD5 context storage. MD5 init/update/final functions mutate this caller-owned struct; string/hash helpers create stack contexts internally.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used by `NoteMD5Init`, `NoteMD5Update`, `NoteMD5Final`, and `NoteMD5Hash`.

## Role In Callers

Carries MD5 state across incremental hashing calls without dynamic allocation.

## Tests Observed

Focused binary-store tests exercise MD5 hash-string behavior through helpers.

## Graph Links

- Graph node id: `typedef:note.h:1523:typedef_1523`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_type` edges to MD5 functions.

## Evidence Gaps

No unresolved source gaps for this node.
