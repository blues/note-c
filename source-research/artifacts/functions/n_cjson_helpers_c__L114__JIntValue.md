# `JIntValue`

## Status

Finalized. Source-inspected integer field accessor.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:114`
- Declaration/interface: `note.h:1310`
- Signature/API shape: `JINTEGER JIntValue(J *item)`
- Visibility: public note-c JSON helper
- Public/exported names: `JIntValue`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `J.valueint`

## Behavior

Returns 0 for a null item. Otherwise returns the node's `valueint` field without checking the node type.

## Important Boundaries

- Preconditions and assumptions: callers that need type safety must check the item type first.
- Error/null/empty behavior: null item returns 0.
- Ownership and lifetime: no allocation; reads caller-owned node.
- Concurrency/global state: no global state.
- Performance shape: O(1).
- Portability/platform constraints: value width follows `JINTEGER`.

## Direct Call Sites

Exact source search found `JGetInt` as the production caller.

## Role In Callers

`JGetInt` uses this helper after verifying the target field is a number.

## Tests Observed

`JIntValue_test.cpp` covers null input, non-integer/non-number object input returning the raw zero field, and a numeric node returning its integer value.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:114:JIntValue`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
