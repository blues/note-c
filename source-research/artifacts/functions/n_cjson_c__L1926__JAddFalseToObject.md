# `JAddFalseToObject`

## Status

Finalized. Source-inspected public create-and-add false-object helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1926`
- Declaration/interface: `n_cjson.h:302`
- Public/exported names: `JAddFalseToObject`
- Macro aliases/wrappers: none found
- Signature/API shape: `J *JAddFalseToObject(J * const object, const char * const name)`
- Visibility: public cJSON helper implemented in `n_cjson.c`
- Ownership/lifetime role: creates a new false item and transfers it into the object on success

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2312:JCreateFalse` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: allocator/free behavior occurs through callees
- Artifact coverage: source-inspected artifact

## Behavior

Returns `NULL` when `object` is `NULL`. Otherwise creates a false item with `JCreateFalse()`, attempts `_add_item_to_object(object, name, false_item, false)`, and returns the created item on successful attach. If attach fails, it calls `JDelete(false_item)` and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `name` is passed to `_add_item_to_object`; a null name causes attach failure and cleanup.
- Error/null/empty behavior: null object, allocation failure, null name, key duplication failure, or array/object attach failure returns `NULL`.
- Ownership and lifetime: on success, the object owns the newly-created child; on failure, the temporary child is deleted before return.
- Concurrency/global state: no direct global state; allocation/free behavior is delegated through callees.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: one item allocation, optional key duplication, and append traversal in `_add_item_to_object`.
- Security or parsing constraints: no parsing; relies on object/list invariants maintained by the JSON tree helpers.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:302` and the definition in `n_cjson.c:1926`. No production repository caller was found.

## Role In Callers

No repository caller role. Public convenience helper for downstream users that need to create and attach a false-valued object field in one call.

## Tests Observed

None. Exact focused test search found no direct `JAddFalseToObject` use.

## Graph Links

- Graph node id: `function:n_cjson.c:1926:JAddFalseToObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JCreateFalse`, `_add_item_to_object`, and `JDelete`.

## Evidence Gaps

No unresolved source gaps for this node.
