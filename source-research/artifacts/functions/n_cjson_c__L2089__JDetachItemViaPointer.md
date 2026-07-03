# `JDetachItemViaPointer`

## Status

Finalized. Source-inspected public JSON child detachment primitive.

## Identity

- Kind: function
- Definition: `n_cjson.c:2089`
- Declaration/interface: `n_cjson.h:272`
- Signature/API shape: `J *JDetachItemViaPointer(J *parent, J * const item)`
- Visibility: public through `N_CJSON_PUBLIC`, whose export/calling-convention shape is configured in `n_cjson.h:134-144`
- Public/exported names: `JDetachItemViaPointer`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: mutates `J.child`, `J.prev`, and `J.next` links in an intrusive child/sibling list
- Artifact coverage: source, header declaration, direct callers, and focused test search inspected

## Behavior

Returns `NULL` when `parent` or `item` is `NULL`. Otherwise detaches `item` from its sibling list:

- if `item->prev` exists, assigns `item->prev->next = item->next`;
- if `item->next` exists, assigns `item->next->prev = item->prev`;
- if `item` is `parent->child`, moves `parent->child` to `item->next`;
- clears `item->prev` and `item->next`;
- returns the detached `item`.

The function does not free the item. Ownership of the detached subtree remains with the caller.

## Important Boundaries

- Preconditions and assumptions: `item`, when non-`NULL`, is assumed to belong to `parent` or at least have sibling links that are safe to mutate.
- Error/null/empty behavior: `NULL` parent or item returns `NULL`; a non-child unrelated item is not rejected.
- Ownership and lifetime: transfers the detached item to the caller without deleting it; deletion wrappers call `JDelete` on the returned pointer.
- Concurrency/global state: no global state or locking.
- Portability/platform constraints: public export macro is platform/config dependent via `N_CJSON_PUBLIC`.
- Performance shape: O(1) pointer rewiring.
- Security or parsing constraints: no membership validation; misuse with an unrelated or corrupted `item` can mutate arbitrary sibling links.

## Direct Call Sites

Exact source search found three production callers:

- `n_cjson.c:2124` `JDetachItemFromArray` finds an array child by index with `_get_array_item`, then detaches it.
- `n_cjson.c:2143` `JDetachItemFromObject` finds an object child with `JGetObjectItem`, then detaches it.
- `n_cjson.c:2154` `JDetachItemFromObjectCaseSensitive` finds an object child with `JGetObjectItemCaseSensitive`, then detaches it.

Deletion wrappers such as `JDeleteItemFromArray` indirectly free detached nodes by calling `JDelete(JDetachItemFromArray(...))`.

## Role In Callers

This is the shared low-level detach primitive for array and object removal APIs. Public wrappers perform lookup from the same parent before calling it, which avoids the missing membership check in normal API use.

## Tests Observed

Focused search found no direct tests for `JDetachItemViaPointer`, `JDetachItemFromArray`, or array deletion wrappers.

## Graph Links

- Graph node id: `function:n_cjson.c:2089:JDetachItemViaPointer`
- Structured graph: `inventory/dependency-graph.json`
- Verified callers: `JDetachItemFromArray`, `JDetachItemFromObject`, `JDetachItemFromObjectCaseSensitive`

## Evidence Gaps

No unresolved source gaps for this node. Focused tests for detach-link rewiring and unrelated-item misuse were not found.
