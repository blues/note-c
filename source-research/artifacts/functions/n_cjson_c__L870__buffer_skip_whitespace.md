# `_buffer_skip_whitespace`

## Status

Finalized. Source-inspected parse-buffer whitespace skipper.

## Identity

- Kind: function
- Definition: `n_cjson.c:870`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `parse_buffer *_buffer_skip_whitespace(parse_buffer * const buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_cjson.c:249:can_access_at_index` finalized
  - `macro:n_cjson.c:252:buffer_at_offset` finalized
- External/system dependencies: none
- Data/type/global dependencies: `parse_buffer.content`, `parse_buffer.offset`, `parse_buffer.length`

## Behavior

Returns NULL for a null buffer or null content. Otherwise advances `offset` while the current byte is `<= 32`, then backs up one byte if the skip ended exactly at `length`, and returns the same buffer pointer.

## Important Boundaries

- Preconditions and assumptions: caller owns mutable parse-buffer offset state.
- Error/null/empty behavior: null buffer/content returns NULL.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: linear over the skipped whitespace run.
- Portability/platform constraints: whitespace is defined as byte values `<= 32`.

## Direct Call Sites

Exact source search found callers in `JParseWithOpts`, `_parse_array`, and `_parse_object`.

## Role In Callers

The root, array, and object parsers use this helper before parsing values and delimiters.

## Tests Observed

Parser-focused public tests exercise this indirectly through `JParse`.

## Graph Links

- Graph node id: `function:n_cjson.c:870:_buffer_skip_whitespace`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
