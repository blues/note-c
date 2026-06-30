# `cjson_min`

## Status

Finalized. Source-inspected local minimum macro.

## Identity

- Kind: macro
- Definition: `n_cjson.c:981`
- Declaration/interface: file-local macro
- Signature/API shape: `cjson_min(a, b)`
- Visibility: internal to `n_cjson.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Expands to `((a < b) ? a : b)`, returning the smaller operand under C expression semantics.

## Important Boundaries

- Preconditions and assumptions: operands may be evaluated more than once.
- Error/null/empty behavior: not applicable.
- Ownership and lifetime: no memory behavior.
- Concurrency/global state: no global state.
- Performance shape: O(1) expression macro.

## Direct Call Sites

Exact source search found `_print` as the only production caller, where it bounds the final `memcpy` from the working print buffer into the tightly sized returned buffer.

## Role In Callers

`_print` uses `cjson_min(buffer->length, buffer->offset + 1)` to avoid reading beyond the current buffer length when copying the final NUL-terminated JSON string.

## Tests Observed

Focused public printing tests exercise this indirectly. No direct macro test was inspected.

## Graph Links

- Graph node id: `macro:n_cjson.c:981:cjson_min`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
