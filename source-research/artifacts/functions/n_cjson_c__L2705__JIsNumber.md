# `JIsNumber`

## Status

Finalized. Source-inspected JSON number predicate.

## Identity

- Kind: function
- Definition: `n_cjson.c:2705`
- Declaration/interface: `n_cjson.h:228`
- Signature/API shape: `Jbool JIsNumber(const J * const item)`
- Visibility: public cJSON/note-c JSON API
- Public/exported names: `JIsNumber`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `J.type`, `JNumber`

## Behavior

Returns false for a null item. Otherwise masks `item->type` with `0xFF` and returns true only when the base type is `JNumber`.

## Important Boundaries

- Preconditions and assumptions: accepts null item.
- Error/null/empty behavior: null and non-number items return false.
- Ownership and lifetime: no allocation; reads caller-owned node.
- Concurrency/global state: no global state.
- Performance shape: O(1).
- Portability/platform constraints: none.

## Direct Call Sites

Exact source search found direct production callers in `JGetNumber`, `JGetInt`, and other JSON helper paths.

## Role In Callers

`JGetInt` uses this predicate to avoid reading `valueint` from non-number fields.

## Tests Observed

`JGetInt_test.cpp`, `JIntValue_test.cpp`, and `JSON_number_handling_test.cpp` exercise this predicate indirectly through integer lookup/value helpers.

## Graph Links

- Graph node id: `function:n_cjson.c:2705:JIsNumber`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused direct `JIsNumber` test was inspected.
