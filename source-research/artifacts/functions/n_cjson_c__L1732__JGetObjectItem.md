# `JGetObjectItem`

## Status

Finalized. Source-inspected public JSON object child lookup wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1732`
- Declaration/interface: `n_cjson.h:213`
- Signature/API shape: `J *JGetObjectItem(const J * const object, const char * const string)`
- Visibility: cJSON API function implemented in `n_cjson.c`
- Public/exported names: `JGetObjectItem`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1710:_get_object_item` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: reads object child list

## Behavior

Returns `NULL` when `object` is `NULL`; otherwise delegates to `_get_object_item(object, string, false)` for case-insensitive lookup.

## Important Boundaries

- Preconditions and assumptions: `string` may be null; lower helper returns `NULL` for null names.
- Error/null/empty behavior: null object/name or missing key returns `NULL`.
- Ownership and lifetime: returns a borrowed pointer into the object tree.
- Concurrency/global state: no global state.
- Performance shape: O(number of children * key length).

## Direct Call Sites

Exact source search found broad helper fanout in `n_cjson_helpers.c`, direct cJSON object detach/has-item callers, and the active `JGetString` lookup path.

## Role In Callers

`JGetString` calls `JGetObjectItem` to find the requested field before checking that it is a string.

## Tests Observed

None. No focused direct test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:1732:JGetObjectItem`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_get_object_item`.

## Evidence Gaps

No unresolved source gaps for this node.
