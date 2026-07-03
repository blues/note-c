# `JDeleteItemFromArray`

## Status

Finalized. Source-inspected public JSON array delete-by-index wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2127`
- Declaration/interface: `n_cjson.h:274`
- Signature/API shape: `void JDeleteItemFromArray(J *array, int which)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JDeleteItemFromArray`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2115:JDetachItemFromArray` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: consumes detached `J` ownership through `JDelete`
- Artifact coverage: source, header declaration, direct caller search, and focused tests inspected

## Behavior

Returns immediately when `array` is `NULL`. Otherwise calls `JDelete(JDetachItemFromArray(array, which))`. Invalid indexes flow through `JDetachItemFromArray` as `NULL`, and `JDelete(NULL)` is a no-op.

The API is void and exposes no success/failure status.

## Important Boundaries

- Preconditions and assumptions: does not validate that `array` has type `JArray`.
- Error/null/empty behavior: null array, missing item, or invalid index results in no deletion and no status.
- Ownership and lifetime: consumes and frees the detached node/tree when an item is found.
- Concurrency/global state: deallocation uses allocator hook state behind `JDelete`/`_Free`.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(index) lookup plus recursive delete cost for the removed subtree.
- Security or parsing constraints: no type validation.

## Direct Call Sites

Exact production search found no direct callers outside its header declaration and implementation. It is a public API entry point.

## Role In Callers

No in-repository production caller was found. Public consumers use it as the consuming/freeing variant of `JDetachItemFromArray`.

## Tests Observed

No focused test references for this symbol were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:2127:JDeleteItemFromArray`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JDetachItemFromArray` and `JDelete`

## Evidence Gaps

No unresolved source gaps for this node. Caller status is explicit none-found in repository source.
