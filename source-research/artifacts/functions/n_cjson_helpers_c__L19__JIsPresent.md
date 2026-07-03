# `JIsPresent`

## Status

Finalized. Source-inspected object field presence predicate.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:19`
- Declaration/interface: `note.h:1196`
- Signature/API shape: `bool JIsPresent(J *json, const char *field)`
- Visibility: public note-c JSON helper
- Public/exported names: `JIsPresent`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
- External/system dependencies: none
- Data/type/global dependencies: object field lookup semantics through `JGetObjectItem`

## Behavior

Returns false for null JSON. Otherwise returns whether `JGetObjectItem(json, field)` returns a non-null item.

## Important Boundaries

- Preconditions and assumptions: `field` is passed through to `JGetObjectItem`; null field follows that helper's behavior.
- Error/null/empty behavior: null JSON or missing field return false.
- Ownership and lifetime: no allocation; reads caller-owned JSON tree.
- Concurrency/global state: no global state.
- Performance shape: object lookup cost.
- Portability/platform constraints: field matching follows `JGetObjectItem` case-insensitive behavior.

## Direct Call Sites

Exact source search found production callers in `_noteTransactionShouldLock`, helper JSON assembly, and focused error-document tests.

## Role In Callers

The transaction path uses `JIsPresent` to gate user-agent injection on absence of `body` and presence of `product`; it also uses related presence checks in raw JSON helper paths.

## Tests Observed

`JIsPresent_test.cpp` covers null JSON, null field, missing field, and present field. `_errDoc_test.cpp` also checks generated error document ID presence.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:19:JIsPresent`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
