# `JDetachItemFromArray`

## Status

Finalized. Source-inspected public JSON array detach-by-index wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2115`
- Declaration/interface: `n_cjson.h:273`
- Signature/API shape: `J *JDetachItemFromArray(J *array, int which)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JDetachItemFromArray`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1681:_get_array_item` finalized
  - `function:n_cjson.c:2089:JDetachItemViaPointer` finalized
- External/system dependencies: none
- Data/type/global dependencies: mutates parent/child/sibling links through `JDetachItemViaPointer`
- Artifact coverage: source, header declaration, direct callers, and focused tests inspected

## Behavior

Returns `NULL` when `array` is `NULL` or `which < 0`. Otherwise finds the indexed child with `_get_array_item` and passes that pointer to `JDetachItemViaPointer`. Missing/out-of-range indexes therefore return `NULL`.

On success, the selected child is unlinked from the parent list and returned with `prev`/`next` cleared. The caller owns the detached item.

## Important Boundaries

- Preconditions and assumptions: does not validate that `array` has type `JArray`.
- Error/null/empty behavior: null array, negative index, empty array, out-of-range index, or missing child returns `NULL`.
- Ownership and lifetime: transfers ownership of the detached item to the caller; does not delete it.
- Concurrency/global state: no global state.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(index) lookup plus O(1) detach.
- Security or parsing constraints: lookup from the same parent prevents normal API use from passing an unrelated item to `JDetachItemViaPointer`.

## Direct Call Sites

Exact source search found one production caller:

- `n_cjson.c:2132` `JDeleteItemFromArray` detaches the indexed child and immediately deletes it with `JDelete`.

## Role In Callers

Provides the ownership-transfer half of the array remove API. `JDeleteItemFromArray` consumes the returned node; public callers may retain it.

## Tests Observed

No focused test references for this symbol were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:2115:JDetachItemFromArray`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_get_array_item` and `JDetachItemViaPointer`

## Evidence Gaps

No unresolved source gaps for this node. Focused detach tests were not found.
