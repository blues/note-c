# `JCreateNumber`

## Status

Finalized. Source-inspected JSON number node constructor.

## Identity

- Kind: function
- Definition: `n_cjson.c:2330`
- Declaration/interface: `n_cjson.h:289`
- Signature/API shape: `J *JCreateNumber(JNUMBER num)`
- Visibility: public cJSON/note-c JSON API
- Public/exported names: `JCreateNumber`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:194:_jNew_Item` finalized
- External/system dependencies: none
- Data/type/global dependencies: `JNumber`, `JINTEGER_MIN`, `JINTEGER_MAX`

## Behavior

Allocates a new `J`, sets type to `JNumber`, stores `valuenumber`, and saturates `valueint` to integer min/max when the number is outside the `JINTEGER` range. Otherwise casts the numeric value to `JINTEGER`. Returns NULL on allocation failure.

## Important Boundaries

- Preconditions and assumptions: numeric conversion follows C casts for in-range values.
- Error/null/empty behavior: allocation failure returns NULL.
- Ownership and lifetime: caller owns the returned node.
- Concurrency/global state: allocator behavior follows note-c hooks through `_jNew_Item`.
- Performance shape: O(1).
- Portability/platform constraints: precision and range follow `JNUMBER` and `JINTEGER`.

## Direct Call Sites

Exact source search found direct callers including `JAddNumberToObject` and number array helpers.

## Role In Callers

`JAddNumberToObject` uses this constructor before transferring the number node into an object field.

## Tests Observed

`JSON_number_handling_test.cpp` exercises this through `JAddNumberToObject` and `JPrintUnformatted`.

## Graph Links

- Graph node id: `function:n_cjson.c:2330:JCreateNumber`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
