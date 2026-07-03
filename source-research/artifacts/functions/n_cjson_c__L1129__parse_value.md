# `_parse_value`

## Status

Finalized. Source-inspected JSON parse dispatcher.

## Identity

- Kind: function
- Definition: `n_cjson.c:1129`
- Declaration/interface: file-local prototype at `n_cjson.c:862`
- Signature/API shape: `Jbool _parse_value(J * const item, parse_buffer * const input_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_cjson.c:247:can_read` finalized
  - `macro:n_cjson.c:252:buffer_at_offset` finalized
  - `macro:n_cjson.c:249:can_access_at_index` finalized
  - `function:n_cjson.c:604:_parse_string` finalized
  - `function:n_cjson.c:255:_parse_number` finalized
  - `function:n_cjson.c:1245:_parse_array` recursive SCC peer
  - `function:n_cjson.c:1395:_parse_object` recursive SCC peer
- External/system dependencies: `strncmp`
- Data/type/global dependencies: JSON literal constants, `J.type`, `J.valueint`

## Behavior

Rejects null item or invalid input buffer, then dispatches by the current input byte. It recognizes `null`, `false`, and `true` literals, setting item type and advancing offset. It delegates strings, numbers, arrays, and objects to their specialized parsers. If no JSON value form matches, returns false.

## Important Boundaries

- Preconditions and assumptions: caller positions the parse buffer at a non-whitespace candidate value byte.
- Error/null/empty behavior: invalid inputs or unmatched syntax return false.
- Ownership and lifetime: scalar literals do not allocate; delegated parsers manage child/string ownership.
- Concurrency/global state: no global state.
- Performance shape: constant dispatch plus delegated parser cost.
- Portability/platform constraints: forms the parser recursion SCC with `_parse_array` and `_parse_object`.

## Direct Call Sites

Exact source search found direct callers in `JParseWithOpts`, `_parse_array`, and `_parse_object`.

## Role In Callers

This is the central JSON parser dispatcher for root values and recursive container children.

## Tests Observed

`JSON_number_handling_test.cpp`, `JPrintUnformatted_test.cpp`, and request/response tests exercise this through `JParse`.

## Graph Links

- Graph node id: `function:n_cjson.c:1129:_parse_value`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused direct tests were identified for each malformed scalar literal variant.
