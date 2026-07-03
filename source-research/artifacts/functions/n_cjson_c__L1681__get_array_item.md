# `_get_array_item`

## Status

Finalized. Source-inspected private JSON array/object child traversal helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1681`
- Declaration/interface: file-local `NOTE_C_STATIC`; no public header declaration
- Signature/API shape: `J *_get_array_item(const J *array, size_t index)`
- Visibility: internal to `n_cjson.c` unless statics are exposed for tests
- Public/exported names: none; public callers expose array index access and mutation

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `J.child` and sibling `J.next` links from the intrusive JSON tree/list
- Artifact coverage: source, declaration absence, direct callers, and focused test search inspected

## Behavior

Returns `NULL` when `array` is `NULL`. Otherwise starts at `array->child` and walks `next` links while `index > 0`, decrementing the index on each step. Returns the child at the requested zero-based position, or `NULL` when the chain ends before that position.

The helper does not check `array->type`; despite its name, it can traverse any `J` node's child list. Public `JGetArrayItem`, `JDetachItemFromArray`, `JInsertItemInArray`, and `JReplaceItemInArray` supply the bounds/type-facing API behavior around this raw traversal.

## Important Boundaries

- Preconditions and assumptions: `array`, when non-`NULL`, points to a valid `J` node whose `child`/`next` links are well formed.
- Error/null/empty behavior: `NULL` array, empty child list, and out-of-range index all return `NULL`.
- Ownership and lifetime: returns a borrowed child pointer; does not allocate, detach, delete, or copy.
- Concurrency/global state: no global state or locking.
- Portability/platform constraints: none beyond ordinary pointer traversal.
- Performance shape: O(index) over the child list.
- Security or parsing constraints: no membership/type validation; callers that expose public APIs must prevent negative indexes before casting to `size_t`.

## Direct Call Sites

Exact source search found four direct callers:

- `n_cjson.c:1707` `JGetArrayItem` rejects `NULL` arrays and negative indexes, then returns the borrowed `_get_array_item` result.
- `n_cjson.c:2124` `JDetachItemFromArray` rejects `NULL` arrays and negative indexes, then passes the found child to `JDetachItemViaPointer`.
- `n_cjson.c:2188` `JInsertItemInArray` uses the found child as the insertion point; a missing child appends through `_add_item_to_array`.
- `n_cjson.c:2248` `JReplaceItemInArray` passes the found child to `JReplaceItemViaPointer`; failure deletes the replacement.

## Role In Callers

This is the shared positional lookup primitive behind public array access, detach, insert, and replace APIs. It intentionally leaves ownership unchanged; higher-level callers decide whether a returned child is borrowed, detached, replaced, or deleted.

## Tests Observed

Focused search found no direct tests for `_get_array_item` or `JGetArrayItem`. Adjacent array tests exercise array creation/retrieval through `JCreateArray`, `JAddItemToObject`, and `JGetArray` in `test/src/JGetArray_test.cpp:21`.

## Graph Links

- Graph node id: `function:n_cjson.c:1681:_get_array_item`
- Structured graph: `inventory/dependency-graph.json`
- Verified callers: `JGetArrayItem`, `JDetachItemFromArray`, `JInsertItemInArray`, `JReplaceItemInArray`

## Evidence Gaps

No unresolved source gaps for this node. Focused tests for index retrieval and out-of-range public behavior were not found.
