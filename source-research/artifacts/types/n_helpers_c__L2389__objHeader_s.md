# `objHeader_s`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: struct field
- Definition: `n_helpers.c:2389`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `objHeader_s` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: recursive `prev` field inside `objHeader_s`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Field belongs to `objHeader` typedef.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Represents the previous-header link inside the internal `objHeader_s` chain; the generated node is field-level rather than a distinct external type.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used by `NoteMemAvailable` through `objHeader *lastObj` and `prev` chain traversal.

## Role In Callers

Allows the memory probe to free all successfully allocated blocks after probing.

## Tests Observed

No focused test found for this internal memory probe.

## Graph Links

- Graph node id: `struct:n_helpers.c:2389:objHeader_s`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_type` edge to `NoteMemAvailable`.

## Evidence Gaps

No unresolved source gaps for this node.
