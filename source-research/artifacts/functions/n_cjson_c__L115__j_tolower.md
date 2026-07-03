# `_j_tolower`

## Status

Finalized. Source-inspected ASCII lowercase helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:115`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `char _j_tolower(char c)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Returns `c` unchanged unless it is in ASCII uppercase range `'A'` through `'Z'`; uppercase characters are converted by adding 32.

## Important Boundaries

- Preconditions and assumptions: ASCII-compatible character constants.
- Error/null/empty behavior: not applicable.
- Ownership and lifetime: no memory access.
- Concurrency/global state: no global state.
- Performance shape: O(1).

## Direct Call Sites

Exact source search found calls from `_case_insensitive_strcmp` at `n_cjson.c:137` and `n_cjson.c:143`.

## Role In Callers

`_case_insensitive_strcmp` uses `_j_tolower` for cJSON object-key case-insensitive comparisons.

## Tests Observed

None. No focused direct test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:115:_j_tolower`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
