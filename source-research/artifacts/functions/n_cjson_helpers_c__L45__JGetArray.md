# `JGetArray`

## Status

Finalized. Source-inspected JSON object array-field accessor, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:45`
- Declaration/interface: `note.h:1238`
- Public/exported names: `JGetArray`
- Macro aliases/wrappers: none
- Signature/API shape: `J *JGetArray(J *json, const char *field)`
- Visibility: public note-c helper API
- Ownership/lifetime role: returns borrowed array child from caller-owned object tree

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
  - `function:n_cjson.c:2721:JIsArray` finalized
- External/system dependencies: none
- Data/type/global dependencies: object child list and `J.type`
- Artifact coverage: source, declaration, direct caller search, and focused tests inspected

## Behavior

Returns `NULL` when `json` is `NULL`, when the named field is absent, or when the item exists but is not an array. Otherwise returns the looked-up `J *` item directly.

## Important Boundaries

- Preconditions and assumptions: `field` may be `NULL`; the finalized `JGetObjectItem` path returns `NULL` for a null key.
- Error/null/empty behavior: null object, missing/null field, and wrong type return `NULL`.
- Ownership and lifetime: returned pointer is borrowed from the parent JSON object and becomes invalid when the parent tree is deleted.
- Concurrency/global state: no global state.
- Portability/platform constraints: none.
- Performance shape: O(number of object children * key length) lookup, then O(1) type check.
- Security or parsing constraints: accepts only items whose base type passes `JIsArray`.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This helper exposes a typed borrowed-array lookup for users of the public API.

## Tests Observed

`test/src/JGetArray_test.cpp` covers null JSON, missing field, wrong-type field, and valid array lookup.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:45:JGetArray`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers `field == NULL`. No production caller was found.
