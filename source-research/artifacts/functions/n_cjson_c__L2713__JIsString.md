# `JIsString`

## Status

Finalized. Source-inspected JSON string type predicate.

## Identity

- Kind: function
- Definition: `n_cjson.c:2713`
- Declaration/interface: `n_cjson.h:229`
- Signature/API shape: `Jbool JIsString(const J * const item)`
- Visibility: cJSON API function implemented in `n_cjson.c`
- Public/exported names: `JIsString`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `item->type`

## Behavior

Returns `false` for `NULL`; otherwise masks `item->type` with `0xFF` and compares it to `JString`.

## Important Boundaries

- Preconditions and assumptions: non-null input points to a valid `J`.
- Error/null/empty behavior: null returns `false`.
- Ownership and lifetime: reads only.
- Concurrency/global state: no global state.
- Performance shape: O(1).

## Direct Call Sites

Exact source search found callers in `n_cjson.c` and string helpers in `n_cjson_helpers.c`, including `JGetString`, `JIsNullString`, `JIsExactString`, and `JContainsString`.

## Role In Callers

`JGetString` uses `JIsString` to reject missing or non-string fields before returning `item->valuestring`.

## Tests Observed

None. No focused direct test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:2713:JIsString`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
