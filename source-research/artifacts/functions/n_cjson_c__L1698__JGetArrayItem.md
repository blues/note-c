# `JGetArrayItem`

## Status

Finalized. Source-inspected public JSON array index accessor.

## Identity

- Kind: function
- Definition: `n_cjson.c:1698`
- Declaration/interface: `n_cjson.h:211`
- Signature/API shape: `J *JGetArrayItem(const J *array, int index)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JGetArrayItem`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1681:_get_array_item` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads `J.child`/`J.next` links through `_get_array_item`
- Artifact coverage: source, header declaration, direct caller search, and focused tests inspected

## Behavior

Returns `NULL` when `array` is `NULL` or `index < 0`. Otherwise casts the index to `size_t` and returns `_get_array_item(array, (size_t)index)`, which yields a borrowed pointer to the child at that zero-based position or `NULL` when out of range.

The API does not check that the parent node has type `JArray`.

## Important Boundaries

- Preconditions and assumptions: non-null parent must have a well-formed child/sibling list.
- Error/null/empty behavior: null array, negative index, empty array, and out-of-range index return `NULL`.
- Ownership and lifetime: returns a borrowed pointer; no allocation, detach, copy, or delete.
- Concurrency/global state: no global state.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(index) over the child list.
- Security or parsing constraints: no type validation.

## Direct Call Sites

Exact production search found no direct callers outside its header declaration and implementation. It is a public API entry point.

## Role In Callers

No in-repository production caller was found. Public consumers can use it for borrowed indexed access into array/object child lists.

## Tests Observed

No focused test references for `JGetArrayItem` were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:1698:JGetArrayItem`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_get_array_item`

## Evidence Gaps

No unresolved source gaps for this node. Caller status is explicit none-found in repository source.
