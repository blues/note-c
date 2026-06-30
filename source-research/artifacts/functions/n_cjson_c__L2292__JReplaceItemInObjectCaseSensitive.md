# `JReplaceItemInObjectCaseSensitive`

## Status

Finalized. Source-inspected public case-sensitive JSON object replacement wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2292`
- Declaration/interface: `n_cjson.h:285`
- Signature/API shape: `void JReplaceItemInObjectCaseSensitive(J *object, const char *string, J *newitem)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JReplaceItemInObjectCaseSensitive`
- Macro aliases/wrappers: none
- Ownership/lifetime role: public cleanup/transfer wrapper around `_replace_item_in_object`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson.c:2253:_replace_item_in_object` finalized in this slice
- External/system dependencies: none directly
- Data/type/global dependencies: mutates object children through `_replace_item_in_object`
- Artifact coverage: generated artifact exists

## Behavior

Deletes `newitem` and returns when `object` or `newitem` is `NULL`. Otherwise calls `_replace_item_in_object(object, string, newitem, true)` for case-sensitive key lookup. If replacement fails, deletes `newitem`; on success, ownership transfers into `object`.

## Important Boundaries

- Preconditions and assumptions: `object` is expected to be an object-like `J`; no type check is performed.
- Error/null/empty behavior: null `string`, exact-key miss, allocation failure, or helper failure consume/delete `newitem`.
- Ownership and lifetime: caller gives `newitem` to this API unconditionally; it is either installed into `object` or deleted.
- Concurrency/global state: cleanup and key allocation use allocator hooks through callees.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(number of object children * key length) plus replacement subtree delete cost.
- Security or parsing constraints: lookup is exact-case `strcmp` through `_get_object_item`; duplicate exact keys resolve to the first matching child.

## Direct Call Sites

No in-repository production call sites were found beyond the definition and public declaration in `n_cjson.h:285`.

## Role In Callers

This is the public case-sensitive object replacement API. It supplies the caller-facing ownership contract around the private helper.

## Tests Observed

None. `rg -n "JReplaceItemInObject|JReplaceItemInObjectCaseSensitive|JReplaceItemViaPointer|ReplaceItem" test/src` found no focused replacement tests.

## Graph Links

- Graph node id: `function:n_cjson.c:2292:JReplaceItemInObjectCaseSensitive`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JDelete` and `_replace_item_in_object`.

## Evidence Gaps

No unresolved source gaps for this node. Focused replacement tests were not found.
