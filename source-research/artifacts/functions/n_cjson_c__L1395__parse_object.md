# `_parse_object`

## Status

Finalized. Source-inspected JSON object parser in the recursive parser SCC.

## Identity

- Kind: function
- Definition: `n_cjson.c:1395`
- Declaration/interface: file-local prototype at `n_cjson.c:866`
- Signature/API shape: `Jbool _parse_object(J * const item, parse_buffer * const input_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_cjson.c:250:cannot_access_at_index` finalized
  - `macro:n_cjson.c:252:buffer_at_offset` finalized
  - `function:n_cjson.c:870:_buffer_skip_whitespace` finalized
  - `macro:n_cjson.c:249:can_access_at_index` finalized
  - `function:n_cjson.c:194:_jNew_Item` finalized
  - `function:n_cjson.c:604:_parse_string` finalized
  - `function:n_cjson.c:1129:_parse_value` recursive SCC peer
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: `parse_buffer.depth`, `N_CJSON_NESTING_LIMIT`, `J.child`, `J.next`, `J.prev`, `J.string`, `J.valuestring`, `JObject`

## Behavior

Rejects excessive nesting, increments parse depth, verifies `{`, skips whitespace, accepts `}` as an empty object, then parses comma-separated key/value members. Each member is allocated with `_jNew_Item`, linked into a child list, parses its key with `_parse_string`, moves the key from `valuestring` to `string`, requires `:`, parses the value through recursive `_parse_value`, and skips whitespace. On closing `}`, it decrements depth, stores `JObject` and the child head on `item`, advances past the delimiter, and returns true. Failures delete any partial child list and return false.

## Important Boundaries

- Preconditions and assumptions: internal callers provide a non-null `item` and parse buffer positioned at `{`.
- Error/null/empty behavior: malformed delimiters, key parse failure, missing colon, child allocation failure, nesting limit, or value parse failure return false.
- Ownership and lifetime: on success `item` owns child members and key strings; on failure partial children are deleted.
- Concurrency/global state: no global state.
- Performance shape: linear in member count plus recursive value parsing.
- Portability/platform constraints: recursion depth is bounded by `N_CJSON_NESTING_LIMIT` during parsing.

## Direct Call Sites

Exact source search found `_parse_value` as the production caller for object values.

## Role In Callers

`JParseWithOpts` reaches this helper through `_parse_value` for root or nested objects.

## Tests Observed

`JPrintUnformatted_test.cpp`, `NoteTransaction_test.cpp`, `NoteRequestResponseJSON_test.cpp`, and helper tests exercise object parsing indirectly through `JParse`.

## Graph Links

- Graph node id: `function:n_cjson.c:1395:_parse_object`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused direct tests were identified for nesting-limit failure, partial-member cleanup, or malformed object-key offset reporting.
