# `JAtoI`

## Status

Finalized. Source-inspected decimal string to `JINTEGER` helper.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:312`
- Declaration/interface: `note.h:1481`
- Signature/API shape: `JINTEGER JAtoI(const char *string)`
- Visibility: public note-c JSON helper
- Public/exported names: `JAtoI`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `JINTEGER`

## Behavior

Skips leading space characters, handles optional `-` or `+`, accumulates base-10 digits until the first non-digit byte, applies the sign, and returns the result.

## Important Boundaries

- Preconditions and assumptions: `string` must be non-null.
- Error/null/empty behavior: no error channel; strings without leading digits after optional sign return 0.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: linear over leading sign/space and digit run.
- Portability/platform constraints: no overflow detection; wrap/overflow behavior follows C integer arithmetic for `JINTEGER`.

## Direct Call Sites

Exact source search found `_parse_number` as the production caller.

## Role In Callers

`_parse_number` uses `JAtoI` to populate `item->valueint` for non-saturated numeric JSON values after `JAtoN` has parsed the floating value.

## Tests Observed

`JAtoI_test.cpp` covers zero, positive numbers, leading zeros/spaces, plus sign, trailing spaces after digits, and negative values.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:312:JAtoI`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
