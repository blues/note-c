# `JInsertItemInArray`

## Status

Finalized. Source-inspected public JSON array insertion wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2174`
- Declaration/interface: `n_cjson.h:281`
- Signature/API shape: `void JInsertItemInArray(J *array, int which, J *newitem)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JInsertItemInArray`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson.c:1681:_get_array_item` finalized
  - `function:n_cjson.c:1783:_add_item_to_array` finalized
- External/system dependencies: none
- Data/type/global dependencies: mutates `J.child`, `J.prev`, and `J.next` links
- Artifact coverage: source, header declaration, direct caller search, and focused tests inspected

## Behavior

Deletes `newitem` and returns when `array` or `newitem` is `NULL`, or when `which < 0`. Otherwise finds the indexed child with `_get_array_item`.

If no child exists at that index, attempts to append `newitem` with `_add_item_to_array`; if append fails, deletes `newitem`. If a child exists, inserts `newitem` before that child by wiring `newitem->next`, `newitem->prev`, the found child's `prev`, and either `array->child` or the previous sibling's `next`.

The API is void and exposes no success/failure status.

## Important Boundaries

- Preconditions and assumptions: does not validate that `array` has type `JArray`; assumes `newitem` can safely be linked.
- Error/null/empty behavior: invalid parent/item/index consumes `newitem` when non-null; out-of-range index appends.
- Ownership and lifetime: on success, parent takes ownership/list membership of `newitem`; on invalid/failure paths, `newitem` is deleted.
- Concurrency/global state: deallocation uses allocator hook state behind `JDelete`/`_Free`.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(index) lookup, or O(children) append when index is out of range.
- Security or parsing constraints: no type validation and no guard that `newitem` is detached before insertion.

## Direct Call Sites

Exact production search found no direct callers outside its header declaration and implementation. It is a public API entry point.

## Role In Callers

No in-repository production caller was found. Public consumers can insert before an existing index or append when the index is beyond the current list.

## Tests Observed

No focused test references for this symbol were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:2174:JInsertItemInArray`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JDelete`, `_get_array_item`, and `_add_item_to_array`

## Evidence Gaps

No unresolved source gaps for this node. Caller status is explicit none-found in repository source.
