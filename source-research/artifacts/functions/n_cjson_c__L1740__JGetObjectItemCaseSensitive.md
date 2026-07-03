# `JGetObjectItemCaseSensitive`

## Status

Finalized. Source-inspected public case-sensitive JSON object lookup wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1740`
- Declaration/interface: `n_cjson.h:214`
- Signature/API shape: `J *JGetObjectItemCaseSensitive(const J * const object, const char * const string)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JGetObjectItemCaseSensitive`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1710:_get_object_item` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: reads object child list through `_get_object_item`
- Artifact coverage: source, header declaration, direct callers, and focused tests inspected

## Behavior

Returns `NULL` when `object` is `NULL`; otherwise delegates to `_get_object_item(object, string, true)`. A `NULL` key or missing key returns `NULL` through the lower helper. The returned pointer is borrowed from the object tree.

## Important Boundaries

- Preconditions and assumptions: does not validate that `object` is actually a JSON object.
- Error/null/empty behavior: null object/name or missing key returns `NULL`.
- Ownership and lifetime: returns a borrowed child pointer with no allocation or ownership transfer.
- Concurrency/global state: no global state.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(number of children * key length).
- Security or parsing constraints: object-child key strings are assumed valid for `strcmp`.

## Direct Call Sites

Exact source search found one production caller:

- `n_cjson.c:2152` `JDetachItemFromObjectCaseSensitive` looks up the named child and detaches it through `JDetachItemViaPointer`.

No focused test references were found under `test/src`.

## Role In Callers

This is the public case-sensitive lookup front end and the lookup primitive for case-sensitive object detach/delete wrappers.

## Tests Observed

None found by exact focused test search for this symbol.

## Graph Links

- Graph node id: `function:n_cjson.c:1740:JGetObjectItemCaseSensitive`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_get_object_item`

## Evidence Gaps

No unresolved source gaps for this node. Focused tests for case-sensitive lookup were not found.
