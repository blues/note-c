# `_replace_item_in_object`

## Status

Finalized. Source-inspected JSON object replacement helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2253`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `Jbool _replace_item_in_object(J *object, const char *string, J *replacement, Jbool case_sensitive)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none
- Macro aliases/wrappers: public wrappers `JReplaceItemInObject` and `JReplaceItemInObjectCaseSensitive`
- Ownership/lifetime role: prepares replacement key ownership and delegates child replacement

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1710:_get_object_item` finalized
  - `function:n_cjson.c:146:_j_strdup` finalized
  - `macro:n_lib.h:242:_Free` finalized
  - `function:n_cjson.c:2206:JReplaceItemViaPointer` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: mutates `replacement->string`, `replacement->type`, and object child links through `JReplaceItemViaPointer`
- Artifact coverage: generated artifact exists

## Behavior

Returns `false` when `object`, `replacement`, or `string` is `NULL`, when `_get_object_item(object, string, case_sensitive)` finds no existing child, or when duplicating `string` with `_j_strdup` fails.

On success, duplicates `string`, releases any existing non-constant `replacement->string`, installs the duplicated key on `replacement`, clears `JStringIsConst` on the replacement type, and calls `JReplaceItemViaPointer(object, existing, replacement)`. It returns `true` after that call.

## Important Boundaries

- Preconditions and assumptions: assumes the found child belongs to `object`; no object type check is performed beyond child lookup.
- Error/null/empty behavior: false return leaves `replacement` ownership with the caller; public wrappers delete it on false.
- Ownership and lifetime: successful replacement transfers `replacement` into `object`; the replaced child is deleted by `JReplaceItemViaPointer`. The replacement key becomes allocator-owned and non-constant even when the replacement previously used `JStringIsConst`.
- Concurrency/global state: allocation/deallocation routes through the SDK allocator hooks.
- Portability/platform constraints: uses SDK `_j_strdup`/`_Free` instead of direct libc allocation.
- Performance shape: O(number of object children * key length) for lookup plus O(key length) duplication and O(1) link replacement.
- Security or parsing constraints: replacement relies on `JReplaceItemViaPointer` membership assumptions after lookup; malformed object child links can still affect pointer rewiring.

## Direct Call Sites

Exact source search found direct callers:

- `n_cjson.c:2287` `JReplaceItemInObject` calls with `case_sensitive=false`.
- `n_cjson.c:2298` `JReplaceItemInObjectCaseSensitive` calls with `case_sensitive=true`.

## Role In Callers

This helper centralizes object-key lookup, key ownership rewrite, and pointer replacement for both public object replacement APIs. It deliberately returns a status instead of deleting `replacement`; cleanup policy lives in the public wrappers.

## Tests Observed

None. `rg -n "JReplaceItemInObject|JReplaceItemInObjectCaseSensitive|JReplaceItemViaPointer|ReplaceItem" test/src` found no focused replacement tests.

## Graph Links

- Graph node id: `function:n_cjson.c:2253:_replace_item_in_object`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_get_object_item`, `_j_strdup`, `_Free`, and `JReplaceItemViaPointer`.

## Evidence Gaps

No unresolved source gaps for this node. Focused replacement tests were not found.
