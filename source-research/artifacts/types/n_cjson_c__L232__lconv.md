# `lconv`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: struct reference
- Definition: `n_cjson.c:232`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `lconv` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: external C library `struct lconv *` local variable
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: External `struct lconv` from locale support.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

This is not a project-defined struct; the extractor captured the locale-library `struct lconv` reference used only when `ENABLE_LOCALES` is defined. `_get_decimal_point` reads `decimal_point[0]`; otherwise it returns `'.'`.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used only by `_get_decimal_point` under `ENABLE_LOCALES`.

## Role In Callers

Provides locale-aware decimal separator lookup when locale support is enabled.

## Tests Observed

No focused test found for `ENABLE_LOCALES` branch.

## Graph Links

- Graph node id: `struct:n_cjson.c:232:lconv`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_type` edge to `_get_decimal_point`.

## Evidence Gaps

No unresolved source gaps for this node.
