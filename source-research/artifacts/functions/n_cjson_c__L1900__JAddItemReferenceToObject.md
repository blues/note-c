# `JAddItemReferenceToObject`

## Status

Finalized. Source-inspected public JSON object reference-add wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1900`
- Declaration/interface: `n_cjson.h:269`
- Signature/API shape: `void JAddItemReferenceToObject(J *object, const char *string, J *item)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JAddItemReferenceToObject`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1764:_create_reference` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: mutates object child list and wrapper key through `_add_item_to_object`
- Artifact coverage: source, header declaration, direct caller search, and focused tests inspected

## Behavior

Returns without action when `object`, `string`, or `item` is `NULL`. Otherwise creates a shallow non-owning reference wrapper with `_create_reference(item)` and attaches that wrapper to `object` under `string` using `_add_item_to_object(..., false)`. If attachment fails, deletes the wrapper with `JDelete`.

The API is void and exposes no success/failure status.

## Important Boundaries

- Preconditions and assumptions: `object` is a valid object-like parent, `string` is a valid key, and referenced `item` remains valid.
- Error/null/empty behavior: null inputs are no-ops; allocation/key-duplication/append failure deletes only the wrapper.
- Ownership and lifetime: on success, the object owns the wrapper node and duplicated key string, not the referenced item's child/value storage.
- Concurrency/global state: allocation/free follows SDK allocator hook state.
- Portability/platform constraints: follows `_create_reference` shallow-copy semantics.
- Performance shape: O(strlen(key) + number of object children).
- Security or parsing constraints: no type validation for `object`.

## Direct Call Sites

Exact production search found no direct callers outside its header declaration and implementation. It is a public API entry point.

## Role In Callers

No in-repository production caller was found. Public consumers can add a borrowed/reference view of an existing item to an object under a duplicated key.

## Tests Observed

No focused test references for this symbol were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:1900:JAddItemReferenceToObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_create_reference`, `_add_item_to_object`, and `JDelete`

## Evidence Gaps

No unresolved source gaps for this node. Caller status is explicit none-found in repository source.
