# `buffer_at_offset`

## Status

Finalized. Source-inspected parse-buffer pointer macro.

## Identity

- Kind: macro
- Definition: `n_cjson.c:252`
- Declaration/interface: file-local macro
- Signature/API shape: `buffer_at_offset(buffer)`
- Visibility: internal to `n_cjson.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `parse_buffer.content`, `parse_buffer.offset`

## Behavior

Returns `buffer->content + buffer->offset`, giving callers a pointer to the current parse position.

## Important Boundaries

- Preconditions and assumptions: caller must ensure `buffer` and `buffer->content` are valid and that the offset is in range.
- Error/null/empty behavior: no null guard.
- Ownership and lifetime: no allocation; returns pointer into caller-owned input.
- Concurrency/global state: no global state.
- Performance shape: O(1).
- Portability/platform constraints: pointer arithmetic over `unsigned char` input.

## Direct Call Sites

Exact source search found uses throughout number, string, root, value, array, and object parsing.

## Role In Callers

All parser helpers use it to read from the current input position after separate bounds checks.

## Tests Observed

Parser-focused public tests exercise this indirectly through `JParse` and JSON helper tests.

## Graph Links

- Graph node id: `macro:n_cjson.c:252:buffer_at_offset`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
