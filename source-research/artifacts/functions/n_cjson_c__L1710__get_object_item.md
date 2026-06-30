# `_get_object_item`

## Status

Finalized. Source-inspected JSON object child lookup helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1710`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `J *_get_object_item(const J * const object, const char * const name, const Jbool case_sensitive)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:127:_case_insensitive_strcmp` finalized
- External/system dependencies: `strcmp` for case-sensitive lookup
- Data/type/global dependencies: reads the child linked list under `object`

## Behavior

Returns `NULL` when `object` or `name` is `NULL`. Otherwise walks `object->child` until it finds a child whose key matches `name`. Case-sensitive mode uses `strcmp`; default mode uses `_case_insensitive_strcmp`. Returns the matching child pointer or `NULL` when none is found.

## Important Boundaries

- Preconditions and assumptions: object child nodes have valid `string` pointers for comparison; the helper does not verify object type.
- Error/null/empty behavior: null object/name returns `NULL`; null child key in case-insensitive mode compares non-equal through `_case_insensitive_strcmp`.
- Ownership and lifetime: returns a borrowed pointer into the parent object.
- Concurrency/global state: no global state.
- Performance shape: O(number of children * key length).

## Direct Call Sites

Exact source search found callers:

- `JGetObjectItem`, `JGetObjectItemCaseSensitive`, object replacement, and `JCompare` in `n_cjson.c`.

## Role In Callers

`JGetObjectItem` uses this helper with case-insensitive lookup. `JGetString` reaches it through `JGetObjectItem`.

## Tests Observed

None. No focused direct test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:1710:_get_object_item`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_case_insensitive_strcmp`.

## Evidence Gaps

No unresolved source gaps for this node.
