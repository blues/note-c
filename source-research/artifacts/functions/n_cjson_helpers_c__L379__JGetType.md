# `JGetType`

## Status

Finalized. Source-inspected JSON object field type-code accessor, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:379`
- Declaration/interface: `note.h:1423`
- Public/exported names: `JGetType`
- Macro aliases/wrappers: none
- Signature/API shape: `int JGetType(J *json, const char *field)`
- Visibility: public note-c helper API
- Ownership/lifetime role: borrowed read of caller-owned object tree

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
  - `function:n_cjson_helpers.c:387:JGetItemType` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: object child list and detailed `JTYPE_*` constants
- Artifact coverage: source, declaration, caller search, and focused tests inspected

## Behavior

Returns `JTYPE_NOT_PRESENT` when `json == NULL` or `field == NULL`. Otherwise performs a case-insensitive object lookup with `JGetObjectItem(json, field)` and returns `JGetItemType` for that item. Missing fields therefore also return `JTYPE_NOT_PRESENT`.

## Important Boundaries

- Preconditions and assumptions: non-null `json` must point to a valid object tree.
- Error/null/empty behavior: null object, null field, and missing field return `JTYPE_NOT_PRESENT`.
- Ownership and lifetime: no allocation; reads borrowed object child.
- Concurrency/global state: no global state.
- Portability/platform constraints: none.
- Performance shape: object lookup is O(number of children * key length), followed by `JGetItemType` classification.
- Security or parsing constraints: detailed classification is delegated to `JGetItemType`, including numeric and boolean string detection.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This helper exposes detailed type classification for fields in public API users' JSON objects.

## Tests Observed

`test/src/JGetType_test.cpp` covers null JSON, null field, missing field, true and false booleans, zero and non-zero numbers, string and raw blank/null/numeric/bool/plain strings, object, array, and invalid item behavior.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:379:JGetType`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers `JTYPE_NULL`, case-insensitive field lookup, masked item flags, or mixed-case boolean strings beyond all-lowercase/all-uppercase cases.
