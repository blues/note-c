# `JAddNumberToObject`

## Status

Finalized. Source-inspected public numeric object-field helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1974`
- Declaration/interface: `n_cjson.h:304`
- Signature/API shape: `J *JAddNumberToObject(J * const object, const char * const name, const JNUMBER number)`
- Visibility: public cJSON/note-c JSON API
- Public/exported names: `JAddNumberToObject`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2330:JCreateNumber` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: object field ownership

## Behavior

Returns NULL for null object. Otherwise creates a number node, attempts to attach it to the object with a duplicated key, returns the new node on success, and deletes the node before returning NULL on failure.

## Important Boundaries

- Preconditions and assumptions: `name` is passed through to `_add_item_to_object`; null name follows that helper's failure path.
- Error/null/empty behavior: null object, allocation failure, or add failure returns NULL.
- Ownership and lifetime: on success object owns the number node; on failure this helper deletes it.
- Concurrency/global state: allocator/free behavior follows note-c hooks.
- Performance shape: object append plus allocation.
- Portability/platform constraints: number storage follows `JCreateNumber`.

## Direct Call Sites

Exact source search found callers in user-agent construction, helper APIs, and tests.

## Role In Callers

`NoteUserAgent` uses this helper for optional CPU numeric fields.

## Tests Observed

`JSON_number_handling_test.cpp`, `JGetInt_test.cpp`, `JGetNumber_test.cpp`, and `NoteUserAgent_test.cpp` exercise this helper indirectly.

## Graph Links

- Graph node id: `function:n_cjson.c:1974:JAddNumberToObject`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
