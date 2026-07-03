# `JAddObjectToObject`

## Status

Finalized. Source-inspected public create-and-add object helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2051`
- Declaration/interface: `n_cjson.h:308`
- Public/exported names: `JAddObjectToObject`
- Macro aliases/wrappers: none found
- Signature/API shape: `J *JAddObjectToObject(J * const object, const char * const name)`
- Visibility: public cJSON helper implemented in `n_cjson.c`
- Ownership/lifetime role: creates a new object item and transfers it into the parent object on success

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2450:JCreateObject` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: allocator/free hooks may be reached through callees
- Data/type/global dependencies: allocator/free behavior occurs through callees
- Artifact coverage: source-inspected artifact

## Behavior

Returns `NULL` when `object` is `NULL`. Otherwise creates an object item with `JCreateObject()`, attempts `_add_item_to_object(object, name, object_item, false)`, and returns the created item on successful attach. If attach fails, it calls `JDelete(object_item)` and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `name` is duplicated by `_add_item_to_object`; target `object` type is not validated.
- Error/null/empty behavior: null object, allocation failure, null name, key duplication failure, or attach failure returns `NULL`.
- Ownership and lifetime: on success, parent object owns the newly-created child and duplicated key; on failure after allocation, the temporary child is deleted.
- Concurrency/global state: no direct global state; allocation/free behavior is delegated through callees.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: one object allocation, key duplication, and append traversal in `_add_item_to_object`.
- Security or parsing constraints: no duplicate-key check and no parent type validation.

## Direct Call Sites

Exact production source search found only the public declaration in `n_cjson.h:308` and the definition in `n_cjson.c:2051`. Focused test/support uses appear under `test/`.

## Role In Callers

No production repository caller role. Test/HITL support uses it to create a nested body object and then write fields into the returned child pointer.

## Tests Observed

Focused evidence in `test/src/JGetType_test.cpp:131` calls `JAddObjectToObject` and asserts `JGetType(json, field) == JTYPE_OBJECT`. HITL support in `test/hitl/card.binary/lib/notecard_binary/NotecardBinary.h:1244` creates a `"body"` child and immediately writes into the returned object pointer.

## Graph Links

- Graph node id: `function:n_cjson.c:2051:JAddObjectToObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JCreateObject`, `_add_item_to_object`, and `JDelete`.

## Evidence Gaps

No unresolved source gaps for this node.
