# `_j_strdup`

## Status

Finalized. Source-inspected JSON string duplication helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:146`
- Declaration/interface: file-local `NOTE_C_STATIC` forward at `n_cjson.c:89`
- Signature/API shape: `unsigned char *_j_strdup(const unsigned char *string)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:241:_Malloc` finalized
- External/system dependencies: `strlen`, `memcpy`
- Data/type/global dependencies: allocates through SDK allocator hook; no global state of its own

## Behavior

Returns `NULL` when `string` is `NULL`. Otherwise computes `strlen(string) + sizeof("")`, allocates that many bytes through `_Malloc`, returns `NULL` if allocation fails, and copies the source string including the NUL terminator into the allocated buffer with `memcpy`.

## Important Boundaries

- Preconditions and assumptions: non-null input must be NUL-terminated.
- Error/null/empty behavior: null input or allocation failure returns `NULL`.
- Ownership and lifetime: returns newly allocated memory owned by the caller; callers free it through `_Free`/`JDelete` paths.
- Concurrency/global state: allocation uses the global allocator hook behind `_Malloc`.
- Portability/platform constraints: avoids direct libc allocation; routes through SDK hook.
- Performance shape: O(strlen(string)).

## Direct Call Sites

Exact source search found `_j_strdup` used in:

- `n_cjson.c:1848` by `_add_item_to_object` to duplicate object keys.
- `n_cjson.c:2264` by object replacement key handling.
- `n_cjson.c:2365` by `JCreateString`.
- `n_cjson.c:2425` by `JCreateRaw`.
- `n_cjson.c:2571` and `n_cjson.c:2577` by `JDuplicate`.

## Role In Callers

In the active JSON add path, `_add_item_to_object` uses `_j_strdup` for non-constant field names and treats `NULL` as attachment failure.

## Tests Observed

None. No focused direct test was inspected for this helper.

## Graph Links

- Graph node id: `function:n_cjson.c:146:_j_strdup`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_Malloc`.

## Evidence Gaps

No unresolved source gaps for this node.
