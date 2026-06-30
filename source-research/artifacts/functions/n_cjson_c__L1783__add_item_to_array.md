# `_add_item_to_array`

## Status

Finalized. Source-inspected JSON child-list append helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1783`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `Jbool _add_item_to_array(J *array, J *item)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1757:_suffix_object` finalized
- External/system dependencies: none
- Data/type/global dependencies: mutates `array->child` or the terminal child link

## Behavior

Returns `false` if `array` or `item` is `NULL`. If `array->child` is `NULL`, stores `item` as the first child. Otherwise walks the `next` chain to the last child and appends `item` using `_suffix_object`. Returns `true` after a successful append.

## Important Boundaries

- Preconditions and assumptions: does not validate that `array` is actually an array/object node or that `item` is unlinked.
- Error/null/empty behavior: null array or item returns `false` and does not free `item`.
- Ownership and lifetime: transfers list membership to the parent on success; allocation/free decisions remain with callers.
- Concurrency/global state: no global state.
- Performance shape: O(number of existing children).

## Direct Call Sites

Exact source search found callers in `n_cjson.c`:

- `n_cjson.c:1814` from `JAddItemToArray`.
- `n_cjson.c:1863` from `_add_item_to_object`.
- `n_cjson.c:1895` from `JAddItemReferenceToArray`.
- `n_cjson.c:2190` from `JInsertItemInArray`.

## Role In Callers

In the active `JAddIntToObject` path, `_add_item_to_object` delegates final child insertion to this helper after preparing the key string and type flags.

## Tests Observed

None. No focused direct test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:1783:_add_item_to_array`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_suffix_object`.

## Evidence Gaps

No unresolved source gaps for this node.
