# `JDelete`

## Status

Finalized. Source-inspected recursive JSON node/tree/list cleanup routine.

## Identity

- Kind: function
- Definition: `n_cjson.c:209`
- Declaration/interface: not found in `note.h`; public-style `N_CJSON_PUBLIC` definition used by SDK sources
- Signature/API shape: `void JDelete(J *item)`
- Visibility: cJSON API function implemented in `n_cjson.c`
- Public/exported names: `JDelete`
- Macro aliases/wrappers: `NoteDeleteResponse(rsp)` in `note.h:486`

## Dependency Status

- Project-local dependencies:
  - Recursive self-call for child cleanup
  - `macro:n_lib.h:242:_Free` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: frees node-owned strings and child lists according to item type flags

## Behavior

Walks a linked list of `J` items. For each item, saves `item->next`, recursively deletes `item->child` when the item is not a reference, frees `valuestring` when the item is not a reference, frees `string` when it is not marked `JStringIsConst`, frees the item itself, and advances to the saved next item.

## Important Boundaries

- Preconditions and assumptions: accepts `NULL` and treats it as an empty list; otherwise expects valid `J` list/tree links and ownership flags.
- Error/null/empty behavior: `NULL` input is a no-op.
- Ownership and lifetime: releases the full owned node/list/tree structure; borrowed/reference children or constant strings are not freed.
- Concurrency/global state: deallocation uses the SDK free hook behind `_Free`.
- Portability/platform constraints: relies on `JIsReference` and `JStringIsConst` flags to preserve borrowed data.
- Performance shape: O(number of linked/tree nodes), recursive by child depth.

## Direct Call Sites

Exact source search found broad production fanout:

- Internal recursive child cleanup at `n_cjson.c:215`.
- JSON parse/print/create/add/replace/delete paths throughout `n_cjson.c`, including typed add wrappers such as `JAddIntToObject`.
- Request/response ownership cleanup in `n_request.c`.
- Binary and high-level helper response/request cleanup in `n_helpers.c`.
- Payload helper cleanup in `n_cjson_helpers.c`.
- Macro wrapper `NoteDeleteResponse(rsp)` in `note.h:486`.

## Role In Callers

In the active `JAddIntToObject` path, `JDelete(integer_item)` releases the newly created integer node if object attachment fails, preventing leaked JSON items.

## Tests Observed

None. No focused direct `JDelete` test was inspected in this slice.

## Graph Links

- Graph node id: `function:n_cjson.c:209:JDelete`
- Structured graph: `inventory/dependency-graph.json`
- Verified cleanup edge to `_Free` and recursive child cleanup semantics.

## Evidence Gaps

No unresolved source gaps for this node.
