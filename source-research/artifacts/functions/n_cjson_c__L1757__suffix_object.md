# `_suffix_object`

## Status

Finalized. Source-inspected JSON doubly-linked-list append helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1757`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `void _suffix_object(J *prev, J *item)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: mutates caller-provided `J` nodes

## Behavior

Links `item` after `prev` by setting `prev->next = item` and `item->prev = prev`.

## Important Boundaries

- Preconditions and assumptions: both pointers must be non-null valid `J` nodes; no null checks.
- Error/null/empty behavior: none.
- Ownership and lifetime: does not allocate or free; it only mutates links.
- Concurrency/global state: no global state.
- Performance shape: O(1).

## Direct Call Sites

Exact source search found callers in `n_cjson.c`:

- `n_cjson.c:1801` from `_add_item_to_array`.
- `n_cjson.c:2481`, `n_cjson.c:2511`, and `n_cjson.c:2541` from array creation helpers.

## Role In Callers

In the active JSON add path, `_add_item_to_array` uses `_suffix_object` after walking to the last child.

## Tests Observed

None. No focused direct test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:1757:_suffix_object`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
