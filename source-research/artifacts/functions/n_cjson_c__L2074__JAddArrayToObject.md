# `JAddArrayToObject`

## Status

Finalized. Source-inspected public JSON object array-field constructor.

## Identity

- Kind: function
- Definition: `n_cjson.c:2074`
- Declaration/interface: `n_cjson.h:309`
- Signature/API shape: `J *JAddArrayToObject(J * const object, const char * const name)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JAddArrayToObject`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2434:JCreateArray` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: creates and attaches an owned `JArray` child
- Artifact coverage: source, header declaration, direct caller search, and focused tests inspected

## Behavior

Returns `NULL` when `object` is `NULL`. Otherwise creates a new array node with `JCreateArray`, attaches it to `object` under `name` using `_add_item_to_object(..., false)`, and returns the array on success. If attachment fails, deletes the array and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `name` must be non-null for attachment to succeed.
- Error/null/empty behavior: null object, allocation failure, null key, key duplication failure, or append failure returns `NULL`.
- Ownership and lifetime: on success, the object owns the returned array child; on failure, the temporary array is deleted.
- Concurrency/global state: allocation/free follows SDK allocator hook state.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(strlen(name) + number of object children).
- Security or parsing constraints: no type validation for `object`.

## Direct Call Sites

Exact production search found no direct callers outside its header declaration and implementation. It is a public API entry point.

## Role In Callers

No in-repository production caller was found. Public consumers use it to create and attach an empty array field in one call.

## Tests Observed

`test/src/JGetType_test.cpp:134-137` exercises this wrapper and expects the field type to be `JTYPE_ARRAY`.

## Graph Links

- Graph node id: `function:n_cjson.c:2074:JAddArrayToObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JCreateArray`, `_add_item_to_object`, and `JDelete`

## Evidence Gaps

No unresolved source gaps for this node. Caller status is explicit none-found in repository source.
