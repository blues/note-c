# `JIsNullString`

## Status

Finalized. Source-inspected null-or-empty string field predicate.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:152`
- Declaration/interface: public note-c JSON helper declaration
- Signature/API shape: `bool JIsNullString(J *json, const char *field)`
- Visibility: public note-c JSON helper
- Public/exported names: `JIsNullString`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
  - `function:n_cjson.c:2713:JIsString` finalized
- External/system dependencies: none
- Data/type/global dependencies: `J.valuestring`

## Behavior

Returns false for null JSON. Looks up `field`; a missing field returns true. A present non-string field returns false. A string field with null `valuestring` or first byte `'\0'` returns true; otherwise false.

## Important Boundaries

- Preconditions and assumptions: `field` is passed through to `JGetObjectItem`.
- Error/null/empty behavior: missing field is treated as null string, while null JSON is false.
- Ownership and lifetime: no allocation; reads caller-owned JSON tree.
- Concurrency/global state: no global state.
- Performance shape: object lookup plus O(1) field checks.
- Portability/platform constraints: field matching follows `JGetObjectItem`.

## Direct Call Sites

Exact source search found `NoteResponseError`, helpers, and tests use this predicate.

## Role In Callers

`NoteResponseError` negates this helper on the `"err"` field, so missing/blank errors are not errors and nonblank string errors are errors.

## Tests Observed

`JIsNullString_test.cpp` covers null JSON, missing field, non-string field, null valuestring, and empty string.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:152:JIsNullString`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
