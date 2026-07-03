# `JAddItemReferenceToArray`

## Status

Finalized. Source-inspected public JSON array reference-add wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1889`
- Declaration/interface: `n_cjson.h:268`
- Signature/API shape: `void JAddItemReferenceToArray(J *array, J *item)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JAddItemReferenceToArray`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1764:_create_reference` finalized
  - `function:n_cjson.c:1783:_add_item_to_array` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: mutates array child list through `_add_item_to_array`
- Artifact coverage: source, header declaration, direct caller search, and focused tests inspected

## Behavior

Returns without action when `array` or `item` is `NULL`. Otherwise creates a shallow non-owning reference wrapper with `_create_reference(item)` and tries to append that wrapper to `array` with `_add_item_to_array`. If append fails, deletes the wrapper with `JDelete`.

The API is void and exposes no success/failure status.

## Important Boundaries

- Preconditions and assumptions: `array` is a valid object/array-like parent and `item` is a valid `J` whose lifetime remains valid.
- Error/null/empty behavior: null array/item is a no-op; allocation failure yields a null wrapper and append failure, then `JDelete(NULL)` no-op.
- Ownership and lifetime: on success, the array owns the wrapper node, not the referenced item's child/value storage.
- Concurrency/global state: allocation/free follows SDK allocator hook state.
- Portability/platform constraints: follows `_create_reference` shallow-copy semantics.
- Performance shape: O(number of existing children) append.
- Security or parsing constraints: no type validation for `array`.

## Direct Call Sites

Exact production search found no direct callers outside its header declaration and implementation. It is a public API entry point.

## Role In Callers

No in-repository production caller was found. Public consumers can add a borrowed/reference view of an existing item to an array.

## Tests Observed

No focused test references for this symbol were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:1889:JAddItemReferenceToArray`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_create_reference`, `_add_item_to_array`, and `JDelete`

## Evidence Gaps

No unresolved source gaps for this node. Caller status is explicit none-found in repository source.
