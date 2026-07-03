# `_parse_array`

## Status

Finalized. Source-inspected JSON array parser in the recursive parser SCC.

## Identity

- Kind: function
- Definition: `n_cjson.c:1245`
- Declaration/interface: file-local prototype at `n_cjson.c:864`
- Signature/API shape: `Jbool _parse_array(J * const item, parse_buffer * const input_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_cjson.c:252:buffer_at_offset` finalized
  - `function:n_cjson.c:870:_buffer_skip_whitespace` finalized
  - `macro:n_cjson.c:249:can_access_at_index` finalized
  - `macro:n_cjson.c:250:cannot_access_at_index` finalized
  - `function:n_cjson.c:194:_jNew_Item` finalized
  - `function:n_cjson.c:1129:_parse_value` recursive SCC peer
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: `parse_buffer.depth`, `N_CJSON_NESTING_LIMIT`, `J.child`, `J.next`, `J.prev`, `JArray`

## Behavior

Rejects excessive nesting, increments parse depth, verifies `[`, skips whitespace, accepts `]` as an empty array, then parses comma-separated child values. Each child is allocated with `_jNew_Item`, linked into a child list, and populated by recursive `_parse_value`. On the closing `]`, it decrements depth, stores `JArray` and the child head on `item`, advances past the delimiter, and returns true. Failures delete any partial child list and return false.

## Important Boundaries

- Preconditions and assumptions: internal callers provide a non-null `item` and parse buffer positioned at `[`.
- Error/null/empty behavior: malformed delimiters, nesting limit, child allocation failure, or child parse failure return false.
- Ownership and lifetime: on success `item` owns the child list; on failure partial children are deleted.
- Concurrency/global state: no global state.
- Performance shape: linear in child count plus recursive value parsing.
- Portability/platform constraints: recursion depth is bounded by `N_CJSON_NESTING_LIMIT` during parsing.

## Direct Call Sites

Exact source search found `_parse_value` as the production caller for array values.

## Role In Callers

`JParseWithOpts` reaches this helper through `_parse_value` for root or nested arrays.

## Tests Observed

`JPrintUnformatted_test.cpp` and request/response parser tests exercise arrays indirectly through `JParse`.

## Graph Links

- Graph node id: `function:n_cjson.c:1245:_parse_array`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused direct tests were identified for nesting-limit failure or partial-child cleanup.
