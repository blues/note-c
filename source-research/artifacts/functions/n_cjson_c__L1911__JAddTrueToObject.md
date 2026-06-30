# `JAddTrueToObject`

## Status

Finalized. Source-inspected public create-and-add true-object helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1911`
- Declaration/interface: `n_cjson.h:301`
- Public/exported names: `JAddTrueToObject`
- Macro aliases/wrappers: none found
- Signature/API shape: `J *JAddTrueToObject(J * const object, const char * const name)`
- Visibility: public cJSON helper implemented in `n_cjson.c`
- Ownership/lifetime role: creates a new true item and transfers it into the object on success

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2303:JCreateTrue` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: allocator/free hooks may be reached through callees
- Data/type/global dependencies: allocator/free behavior occurs through callees
- Artifact coverage: source-inspected artifact

## Behavior

Returns `NULL` when `object` is `NULL`. Otherwise creates a true item with `JCreateTrue()`, attempts `_add_item_to_object(object, name, true_item, false)`, and returns the created item on successful attach. If attach fails, it calls `JDelete(true_item)` and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `name` is duplicated by `_add_item_to_object` because this helper uses the non-constant-key path.
- Error/null/empty behavior: null object, allocation failure, null name, key duplication failure, or array/object attach failure returns `NULL`.
- Ownership and lifetime: on success, the object owns the newly-created child and duplicated key; on failure, the temporary child is deleted before return.
- Concurrency/global state: no direct global state; allocation/free behavior is delegated through callees.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: one item allocation, key duplication, and append traversal in `_add_item_to_object`.
- Security or parsing constraints: no parsing; does not validate the target object's type before mutating its child list.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:301` and the definition in `n_cjson.c:1911`. No production repository caller was found. `JAddBoolToObject` is a sibling API and does not delegate to this helper.

## Role In Callers

No repository caller role. Public convenience helper for downstream users that need to create and attach a true-valued object field in one call.

## Tests Observed

None. Exact focused test search found no direct `JAddTrueToObject` use. Adjacent boolean tests exercise `JAddBoolToObject`, which does not call this function.

## Graph Links

- Graph node id: `function:n_cjson.c:1911:JAddTrueToObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JCreateTrue`, `_add_item_to_object`, and `JDelete`.

## Evidence Gaps

No unresolved source gaps for this node.
