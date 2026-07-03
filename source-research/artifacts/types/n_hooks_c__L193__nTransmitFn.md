# `nTransmitFn`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `n_hooks.c:193`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `nTransmitFn` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: `const char *(*)(const uint8_t *, uint32_t, bool)`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Internal function pointer type.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the internal active-interface function-pointer slot type used by `n_hooks.c` to route hard reset, JSON transaction, chunked receive, or chunked transmit through the selected I2C/serial implementation.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used by the corresponding `notecard*` static function pointer global and assigned by `_noteSetActiveInterface`.

## Role In Callers

Keeps platform-specific transport dispatch behind typed internal hooks.

## Tests Observed

Focused hook/transport tests exercise active interface selection and dispatch behavior.

## Graph Links

- Graph node id: `typedef:n_hooks.c:193:nTransmitFn`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_type` edge to `_noteSetActiveInterface`.

## Evidence Gaps

No unresolved source gaps for this node.
