# `_case_insensitive_strcmp`

## Status

Finalized. Source-inspected case-insensitive cJSON key comparison helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:127`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `int _case_insensitive_strcmp(const unsigned char *string1, const unsigned char *string2)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:115:_j_tolower` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads caller-provided NUL-terminated strings

## Behavior

Returns `1` if either input is `NULL`, `0` if the two pointers are identical, otherwise compares the strings byte by byte after applying `_j_tolower`. It returns `0` when both strings reach NUL together or the difference between lowercased mismatching bytes.

## Important Boundaries

- Preconditions and assumptions: non-null inputs must be NUL-terminated strings.
- Error/null/empty behavior: two NULL pointers are not considered equal; any NULL input returns nonzero.
- Ownership and lifetime: reads caller memory only.
- Concurrency/global state: no global state.
- Portability/platform constraints: ASCII-only lowercasing through `_j_tolower`.
- Performance shape: O(length of common prefix).

## Direct Call Sites

Exact source search found `_get_object_item` as the production caller at `n_cjson.c:1724`.

## Role In Callers

`_get_object_item` uses this helper for default case-insensitive object-key lookup.

## Tests Observed

None. No focused direct test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:127:_case_insensitive_strcmp`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_j_tolower`.

## Evidence Gaps

No unresolved source gaps for this node.
