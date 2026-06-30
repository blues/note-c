# `_Malloc`

## Status

Finalized. Source-inspected transparent allocation macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:241`
- Signature/API shape: `#define _Malloc NoteMalloc`
- Visibility: private/internal alias in `n_lib.h`
- Public/exported names: none; public implementation is `NoteMalloc`
- Macro aliases/wrappers: transparent alias, no argument adaptation

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:569:NoteMalloc` finalized
- External/system dependencies: inherited platform allocation hook
- Data/type/global dependencies: inherited from `NoteMalloc`

## Behavior

Expands internal `_Malloc(...)` calls directly to `NoteMalloc(...)`. It does not change arguments, ownership, return value, or error behavior.

## Important Boundaries

- Preconditions and assumptions: same as `NoteMalloc`.
- Error/null/empty behavior: same as `NoteMalloc`; returns `NULL` if no allocator hook exists or allocation fails.
- Ownership and lifetime: allocated memory ownership transfers to caller.
- Concurrency/global state: same hook-global boundary as `NoteMalloc`.
- Portability/platform constraints: keeps internal allocation calls behind SDK hook API.

## Direct Call Sites

Exact source search found `_Malloc` use in:

- `n_cjson.c` for string duplication, item allocation, parse/print buffers, and object replacement.
- `n_request.c` for request/response JSON buffers and CRC augmentation.
- `n_i2c.c` and `n_serial.c` for transaction JSON buffers.
- `n_helpers.c` for payload/base64 and memory-availability helper structures.

## Role In Callers

Internal modules use `_Malloc` as the portable allocation boundary. In the active JSON slice, `_j_strdup` and `_jNew_Item` depend on this alias to allocate duplicated object keys and `J` nodes.

## Tests Observed

None. Tests were not researched as standalone allocator targets.

## Graph Links

- Graph node id: `macro:n_lib.h:241:_Malloc`
- Structured graph: `inventory/dependency-graph.json`
- Verified expansion edge to `function:n_hooks.c:569:NoteMalloc`.

## Evidence Gaps

No unresolved source gaps for this node.
