# `can_read`

## Status

Finalized. Source-inspected parse-buffer bounds macro.

## Identity

- Kind: macro
- Definition: `n_cjson.c:247`
- Declaration/interface: file-local macro
- Signature/API shape: `can_read(buffer, size)`
- Visibility: internal to `n_cjson.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `parse_buffer.offset`, `parse_buffer.length`

## Behavior

Returns true when `buffer` is non-null and `buffer->offset + size <= buffer->length`. The macro is used for fixed-length literal checks during parsing.

## Important Boundaries

- Preconditions and assumptions: `size` should be a bounded expression; macro arguments may be evaluated more than once.
- Error/null/empty behavior: null buffer returns false.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(1).
- Portability/platform constraints: arithmetic is on `size_t` fields.

## Direct Call Sites

Exact source search found `_parse_value` uses `can_read` for `null`, `false`, and `true` literal checks.

## Role In Callers

It prevents fixed-length literal comparisons from reading beyond the current parse buffer.

## Tests Observed

Parser-focused public tests exercise this indirectly through `JParse` and JSON helper tests.

## Graph Links

- Graph node id: `macro:n_cjson.c:247:can_read`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
