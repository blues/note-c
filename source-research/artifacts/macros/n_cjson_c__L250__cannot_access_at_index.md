# `cannot_access_at_index`

## Status

Finalized. Source-inspected inverse parse-buffer access macro.

## Identity

- Kind: macro
- Definition: `n_cjson.c:250`
- Declaration/interface: file-local macro
- Signature/API shape: `cannot_access_at_index(buffer, index)`
- Visibility: internal to `n_cjson.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_cjson.c:249:can_access_at_index` finalized
- External/system dependencies: none
- Data/type/global dependencies: parse-buffer offset/length through `can_access_at_index`

## Behavior

Expands to the logical negation of `can_access_at_index(buffer, index)`.

## Important Boundaries

- Preconditions and assumptions: inherits macro argument evaluation behavior from `can_access_at_index`.
- Error/null/empty behavior: null buffer returns true through the negated helper.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(1).
- Portability/platform constraints: none beyond `can_access_at_index`.

## Direct Call Sites

Exact source search found uses in `_parse_array` and `_parse_object`.

## Role In Callers

Container parsers use this macro to fail cleanly on truncated arrays and objects.

## Tests Observed

Parser-focused public tests exercise this indirectly through `JParse` and JSON helper tests.

## Graph Links

- Graph node id: `macro:n_cjson.c:250:cannot_access_at_index`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
