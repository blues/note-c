# `_Free`

## Status

Finalized. Source-inspected transparent deallocation macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:242`
- Signature/API shape: `#define _Free NoteFree`
- Visibility: private/internal alias in `n_lib.h`
- Public/exported names: none; public implementation is `NoteFree`
- Macro aliases/wrappers: transparent alias, no argument adaptation

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:595:NoteFree` finalized
- External/system dependencies: inherited platform free hook
- Data/type/global dependencies: inherited from `NoteFree`

## Behavior

Expands internal `_Free(...)` calls directly to `NoteFree(...)`. It does not change arguments, return behavior, or ownership semantics.

## Important Boundaries

- Preconditions and assumptions: same as `NoteFree`.
- Error/null/empty behavior: same as `NoteFree`; missing hook is a silent no-op.
- Ownership and lifetime: releases caller-provided memory according to the registered hook.
- Concurrency/global state: same hook-global boundary as `NoteFree`.
- Portability/platform constraints: keeps internal deallocation calls behind SDK hook API.

## Direct Call Sites

Exact source search found `_Free` use in:

- `n_cjson.c` for JSON string/node/buffer cleanup and object replacement.
- `n_request.c` for request/response JSON buffers.
- `n_i2c.c` and `n_serial.c` for transport JSON buffers.
- `n_helpers.c` for payload/base64, memory-availability, and payload segment cleanup.
- `n_cjson_helpers.c` for binary/base64 helper cleanup.

## Role In Callers

In the active JSON add path, `_add_item_to_object` uses `_Free` to release an existing non-constant `item->string` before replacing it with the new object key.

## Tests Observed

None. Tests were not researched as standalone deallocator targets.

## Graph Links

- Graph node id: `macro:n_lib.h:242:_Free`
- Structured graph: `inventory/dependency-graph.json`
- Verified expansion edge to `function:n_hooks.c:595:NoteFree`.

## Evidence Gaps

No unresolved source gaps for this node.
