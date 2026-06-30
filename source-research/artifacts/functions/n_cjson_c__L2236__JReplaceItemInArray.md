# `JReplaceItemInArray`

## Status

Finalized. Source-inspected public JSON array replace-by-index wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2236`
- Declaration/interface: `n_cjson.h:283`
- Signature/API shape: `void JReplaceItemInArray(J *array, int which, J *newitem)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JReplaceItemInArray`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson.c:1681:_get_array_item` finalized
  - `function:n_cjson.c:2206:JReplaceItemViaPointer` finalized
- External/system dependencies: none
- Data/type/global dependencies: mutates parent/child/sibling links through `JReplaceItemViaPointer`
- Artifact coverage: source, header declaration, direct caller search, and focused tests inspected

## Behavior

Deletes `newitem` and returns when `array` or `newitem` is `NULL`, or when `which < 0`. Otherwise finds the indexed child with `_get_array_item` and attempts replacement with `JReplaceItemViaPointer`. If replacement fails, deletes `newitem`.

Out-of-range indexes therefore consume/delete `newitem`, because `_get_array_item` returns `NULL` and `JReplaceItemViaPointer` returns `false`.

The API is void and exposes no success/failure status.

## Important Boundaries

- Preconditions and assumptions: does not validate that `array` has type `JArray`.
- Error/null/empty behavior: null parent/item, negative index, or out-of-range index deletes `newitem` when non-null and returns no status.
- Ownership and lifetime: on success, parent takes ownership/list membership of `newitem` and old item is deleted; on failure, `newitem` is deleted.
- Concurrency/global state: deallocation uses allocator hook state behind `JDelete`/`_Free`.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(index) lookup plus O(1) pointer replacement and old-subtree delete cost.
- Security or parsing constraints: no type validation.

## Direct Call Sites

Exact production search found no direct callers outside its header declaration and implementation. It is a public API entry point.

## Role In Callers

No in-repository production caller was found. Public consumers use it as the consuming array replacement API.

## Tests Observed

No focused test references for this symbol were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:2236:JReplaceItemInArray`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_get_array_item`, `JReplaceItemViaPointer`, and `JDelete`

## Evidence Gaps

No unresolved source gaps for this node. Caller status is explicit none-found in repository source.
