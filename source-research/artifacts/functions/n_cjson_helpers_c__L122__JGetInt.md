# `JGetInt`

## Status

Finalized. Source-inspected object integer field accessor.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:122`
- Declaration/interface: `note.h:1266`
- Signature/API shape: `JINTEGER JGetInt(J *json, const char *field)`
- Visibility: public note-c JSON helper
- Public/exported names: `JGetInt`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
  - `function:n_cjson.c:2705:JIsNumber` finalized
  - `function:n_cjson_helpers.c:114:JIntValue` finalized
- External/system dependencies: none
- Data/type/global dependencies: object field map/list semantics through `JGetObjectItem`

## Behavior

Returns 0 for null `json`, missing field, or a field whose item is not a number. Otherwise returns `JIntValue(item)`.

## Important Boundaries

- Preconditions and assumptions: `field` is passed through to `JGetObjectItem`; null field follows that helper's behavior.
- Error/null/empty behavior: all missing/wrong-type paths collapse to 0.
- Ownership and lifetime: no allocation; reads caller-owned JSON tree.
- Concurrency/global state: no global state.
- Performance shape: object lookup cost plus O(1) checks.
- Portability/platform constraints: returned width follows `JINTEGER`.

## Direct Call Sites

Exact source search found many production callers, including `_noteTransactionShouldLock` for request IDs, timeout calculation, environment helpers, user-agent tests, and helper APIs.

## Role In Callers

`_noteTransactionShouldLock` uses `JGetInt(req, "id")` so generated error response documents can preserve the request ID.

## Tests Observed

`JGetInt_test.cpp` covers null JSON, missing field, wrong type, and numeric field. `JSON_number_handling_test.cpp` covers integer bounds through parse and lookup.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:122:JGetInt`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test was inspected for null `field`.
