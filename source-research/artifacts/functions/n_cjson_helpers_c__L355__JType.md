# `JType`

## Status

Finalized. Source-inspected JSON item type-name mapper, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:355`
- Declaration/interface: `note.h:1398`
- Public/exported names: `JType`
- Macro aliases/wrappers: none
- Signature/API shape: `const char *JType(J *item)`
- Visibility: public note-c helper API
- Ownership/lifetime role: returns pointers to static string literals

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `J.type` and public JSON type constants
- Artifact coverage: source, declaration, caller search, and focused tests inspected

## Behavior

Returns `""` for `NULL` input. Otherwise switches on `item->type & 0xff`, ignoring high-bit item flags. `JTrue` and `JFalse` map to `"bool"`, `JNULL` to `"null"`, `JNumber` to `"number"`, `JRaw` and `JString` to `"string"`, `JObject` to `"object"`, `JArray` to `"array"`, and all other base types to `"invalid"`.

## Important Boundaries

- Preconditions and assumptions: non-null input must point to a valid `J`.
- Error/null/empty behavior: null item returns `""`; invalid or unknown base types return `"invalid"`.
- Ownership and lifetime: returned strings are static literals and must not be freed.
- Concurrency/global state: no global state.
- Portability/platform constraints: none.
- Performance shape: O(1).
- Security or parsing constraints: type flags are masked out before mapping.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This helper exposes human-readable base type names for public API users.

## Tests Observed

`test/src/JType_test.cpp` covers null item, invalid item, true and false bools, number, string, raw-as-string, object, and array.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:355:JType`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers `JNULL` or proves high-bit flags are masked.
