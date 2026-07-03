# `JAddItemToArray`

## Status

Finalized. Source-inspected public array attach helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1808`
- Declaration/interface: `n_cjson.h:261`
- Public/exported names: `JAddItemToArray`
- Macro aliases/wrappers: none found
- Signature/API shape: `void JAddItemToArray(J *array, J *item)`
- Visibility: public cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: transfers `item` into the array/object child chain on success; consumes/deletes `item` on failure

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson.c:1783:_add_item_to_array` finalized
- External/system dependencies: free hook may be reached on cleanup through `JDelete` / `_Free`
- Data/type/global dependencies: mutates `array->child`, `item->prev`, and linked-list `next` fields through `_add_item_to_array`
- Artifact coverage: source-inspected artifact

## Behavior

If `array` or `item` is `NULL`, calls `JDelete(item)` and returns. Otherwise calls `_add_item_to_array(array, item)`. If the helper reports failure, deletes `item`; on success it returns with `item` linked into the array/object child list.

## Important Boundaries

- Preconditions and assumptions: does not verify `array->type`; any non-null `J *` can have its child list mutated.
- Error/null/empty behavior: invalid inputs consume/delete `item` when non-null; `JDelete(NULL)` is a no-op by loop shape.
- Ownership and lifetime: successful attach makes `item` part of `array`'s child chain so later parent deletion reaches it.
- Concurrency/global state: no direct global state; cleanup may use the configured free hook through `JDelete`.
- Portability/platform constraints: no platform-specific behavior.
- Performance shape: O(number of existing children) append traversal for non-empty child chains.
- Security or parsing constraints: does not clear or validate pre-existing `item->next`/`item->prev`; relies on callers supplying a detached item.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:261` and the definition in `n_cjson.c:1808`. No production repository caller was found.

## Role In Callers

No repository caller role. Public cJSON-compatible helper for downstream code that transfers a caller-created item into an array/object child list.

## Tests Observed

None. Focused searches found no direct tests for `JAddItemToArray` or `_add_item_to_array`.

## Graph Links

- Graph node id: `function:n_cjson.c:1808:JAddItemToArray`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JDelete` and `_add_item_to_array`.

## Evidence Gaps

No unresolved source gaps for this node.
