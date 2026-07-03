# `JAddItemToObject`

## Status

Finalized. Source-inspected public object child ownership wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1866`
- Declaration/interface: `n_cjson.h:262`
- Signature/API shape: `void JAddItemToObject(J *object, const char *string, J *item)`
- Visibility: public cJSON/note-c JSON API
- Public/exported names: `JAddItemToObject`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
- External/system dependencies: none
- Data/type/global dependencies: `J` object/list ownership

## Behavior

Deletes `item` and returns when `object`, `string`, or `item` is null. Otherwise delegates to `_add_item_to_object(object, string, item, false)`, which duplicates the key and appends the item. If the lower-level add fails, the wrapper deletes `item`.

## Important Boundaries

- Preconditions and assumptions: caller transfers ownership of `item` on every non-null call path.
- Error/null/empty behavior: invalid inputs or add failure do not report status; they clean up `item` and return.
- Ownership and lifetime: on success, object owns `item`; on failure, `item` is deleted.
- Concurrency/global state: allocator/free behavior follows JSON hook configuration through callees.
- Performance shape: bounded by key duplication plus append traversal in `_add_item_to_object`.
- Portability/platform constraints: none beyond JSON allocator hooks.

## Direct Call Sites

Exact source search found production callers in binary helper JSON assembly, user-agent injection in `_noteTransactionShouldLock`, and helper macros such as `NoteAddBodyToObject`. Focused tests call it in JSON helper tests and invalid-item type classification.

## Role In Callers

`_noteTransactionShouldLock` uses this wrapper to attach the generated user-agent body to a request object when the hub.set/product predicate passes.

## Tests Observed

`JGetArray_test.cpp`, `JGetObject_test.cpp`, and `JGetType_test.cpp` exercise successful attachment. `NoteTransaction_test.cpp` covers user-agent transaction cases that reach this wrapper indirectly.

## Graph Links

- Graph node id: `function:n_cjson.c:1866:JAddItemToObject`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test was inspected for the wrapper deleting `item` on null object/key or `_add_item_to_object` failure.
