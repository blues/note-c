# `JDetachItemFromObject`

## Status

Finalized. Source-inspected public JSON object detach-by-key wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2135`
- Declaration/interface: `n_cjson.h:275`
- Signature/API shape: `J *JDetachItemFromObject(J *object, const char *string)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JDetachItemFromObject`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
  - `function:n_cjson.c:2089:JDetachItemViaPointer` finalized
- External/system dependencies: none
- Data/type/global dependencies: mutates object child/sibling links through `JDetachItemViaPointer`
- Artifact coverage: source, header declaration, direct callers, and focused tests inspected

## Behavior

Returns `NULL` when `object` is `NULL`. Otherwise performs a case-insensitive lookup with `JGetObjectItem(object, string)` and passes the result to `JDetachItemViaPointer`. A `NULL` key or missing key returns `NULL` through lookup/detach.

On success, the matched child is unlinked from the object and returned with `prev`/`next` cleared. The caller owns the detached item.

## Important Boundaries

- Preconditions and assumptions: does not validate that `object` has type `JObject`.
- Error/null/empty behavior: null object/name or missing key returns `NULL`.
- Ownership and lifetime: transfers detached item ownership to the caller.
- Concurrency/global state: no global state.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(number of children * key length) lookup plus O(1) detach.
- Security or parsing constraints: normal wrapper use finds the child from the same parent before detaching.

## Direct Call Sites

Exact source search found production callers:

- `n_cjson.c:2162` `JDeleteItemFromObject` deletes the detached item.
- `n_helpers.c:2146` detaches `"body"` from a response object in helper response handling.

## Role In Callers

Provides the ownership-transfer half of object key removal and supports callers that need to keep or reparent a detached subtree.

## Tests Observed

No focused test references for this symbol were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:2135:JDetachItemFromObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JGetObjectItem` and `JDetachItemViaPointer`

## Evidence Gaps

No unresolved source gaps for this node.
