# `can_access_at_index`

## Status

Finalized. Source-inspected parse-buffer indexed access macro.

## Identity

- Kind: macro
- Definition: `n_cjson.c:249`
- Declaration/interface: file-local macro
- Signature/API shape: `can_access_at_index(buffer, index)`
- Visibility: internal to `n_cjson.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `parse_buffer.offset`, `parse_buffer.length`

## Behavior

Returns true when `buffer` is non-null and `buffer->offset + index < buffer->length`. It is the parser's general guard before reading a byte at an offset-relative index.

## Important Boundaries

- Preconditions and assumptions: macro arguments may be evaluated more than once.
- Error/null/empty behavior: null buffer returns false.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(1).
- Portability/platform constraints: arithmetic is on `size_t` fields.

## Direct Call Sites

Exact source search found uses in `_parse_number`, `_buffer_skip_whitespace`, `_skip_utf8_bom`, `_parse_value`, `_parse_array`, and `_parse_object`.

## Role In Callers

It is the parser's primary byte-availability guard for scalar and container parsers.

## Tests Observed

Parser-focused public tests exercise this indirectly through `JParse` and JSON helper tests.

## Graph Links

- Graph node id: `macro:n_cjson.c:249:can_access_at_index`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
