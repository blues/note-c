# `JMalloc`

## Status

Finalized. Source-inspected public CJSON allocation wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:175`
- Declaration/interface: `n_cjson.h:325`
- Public/exported names: `JMalloc`
- Macro aliases/wrappers: none for `JMalloc`; implementation delegates through internal `_Malloc` alias in `n_lib.h:241`
- Signature/API shape: `void *JMalloc(size_t size)`
- Visibility: public cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: allocates through the SDK allocator hook and transfers returned memory to caller

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:241:_Malloc` expands to finalized `NoteMalloc`
- External/system dependencies: allocator hook used by `NoteMalloc`
- Data/type/global dependencies: reads allocator hook state through `NoteMalloc`
- Artifact coverage: source-inspected artifact

## Behavior

Returns `_Malloc(size)`, where `_Malloc` is the internal alias for `NoteMalloc`. If no allocation hook is registered or the hook fails, the delegated call returns `NULL`; otherwise the returned pointer is passed through unchanged.

## Important Boundaries

- Preconditions and assumptions: allocation semantics are supplied by the registered platform allocator hook.
- Error/null/empty behavior: no hook or hook failure returns `NULL`; no error string is produced.
- Ownership and lifetime: returned allocation belongs to the caller and should be freed with the paired SDK free path.
- Concurrency/global state: delegates to `NoteMalloc`, which reads hook globals without taking the Notecard lock.
- Portability/platform constraints: allocation policy stays behind SDK hooks rather than libc calls in this wrapper.
- Performance shape: one allocator wrapper call plus any optional debug work in `NoteMalloc`.
- Security or parsing constraints: no parsing or bounds checks; requested size is passed through unchanged.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:325` and the definition in `n_cjson.c:175`. No production or test caller was found in repository source.

## Role In Callers

No repository caller role. This function exposes the SDK allocator hook through the public cJSON-compatible API surface.

## Tests Observed

None. Exact focused test search found no direct `JMalloc` use.

## Graph Links

- Graph node id: `function:n_cjson.c:175:JMalloc`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_Malloc` / `NoteMalloc`.

## Evidence Gaps

No unresolved source gaps for this node.
