# `JReplaceItemViaPointer`

## Status

Finalized. Source-inspected public JSON replacement primitive by child pointer.

## Identity

- Kind: function
- Definition: `n_cjson.c:2206`
- Declaration/interface: `n_cjson.h:282`
- Signature/API shape: `Jbool JReplaceItemViaPointer(J * const parent, J * const item, J *replacement)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JReplaceItemViaPointer`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: mutates `J.child`, `J.prev`, and `J.next` links
- Artifact coverage: source, header declaration, direct callers, and focused tests inspected

## Behavior

Returns `false` when `parent`, `item`, or `replacement` is `NULL`, without freeing `replacement`. If `replacement == item`, returns `true` as a no-op.

Otherwise copies the replaced item's sibling links to `replacement`, updates adjacent siblings to point at `replacement`, updates `parent->child` when replacing the first child, clears the old item's `next`/`prev`, deletes the old item with `JDelete`, and returns `true`.

## Important Boundaries

- Preconditions and assumptions: assumes `item` belongs to `parent` or at least has sibling links safe to mutate.
- Error/null/empty behavior: null inputs return `false`; replacement ownership remains with the caller on `false`.
- Ownership and lifetime: on success, parent takes ownership/list membership of `replacement` and the replaced item is deleted; on `replacement == item`, ownership is unchanged.
- Concurrency/global state: deallocation uses allocator hook state behind `JDelete`/`_Free`.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(1) pointer rewiring plus recursive delete cost for the old subtree.
- Security or parsing constraints: no membership validation; misuse with unrelated/corrupted links can mutate arbitrary sibling links.

## Direct Call Sites

Exact source search found production callers:

- `n_cjson.c:2248` `JReplaceItemInArray` replaces the child at an array index.
- `n_cjson.c:2276` `_replace_item_in_object` replaces an object child after key lookup and replacement-key preparation.

## Role In Callers

This is the shared low-level replace primitive for array and object replace APIs. Higher wrappers provide lookup and failure cleanup for `replacement`.

## Tests Observed

No focused test references for this symbol were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:2206:JReplaceItemViaPointer`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `JDelete`

## Evidence Gaps

No unresolved source gaps for this node. Focused replacement tests were not found.
