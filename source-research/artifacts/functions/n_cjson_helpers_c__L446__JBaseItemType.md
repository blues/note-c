# `JBaseItemType`

## Status

Finalized. Source-inspected detailed-to-base JSON type-code mapper, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:446`
- Declaration/interface: `note.h:1439`
- Public/exported names: `JBaseItemType`
- Macro aliases/wrappers: none
- Signature/API shape: `int JBaseItemType(int type)`
- Visibility: public note-c helper API
- Ownership/lifetime role: pure scalar mapping

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: public `JTYPE_*` constants
- Artifact coverage: source, declaration, caller search, and focused tests inspected

## Behavior

Coalesces detailed type codes to base codes: `JTYPE_BOOL_TRUE` and `JTYPE_BOOL_FALSE` return `JTYPE_BOOL`; `JTYPE_NUMBER_ZERO` returns `JTYPE_NUMBER`; `JTYPE_STRING_BLANK`, `JTYPE_STRING_ZERO`, `JTYPE_STRING_NUMBER`, `JTYPE_STRING_BOOL_TRUE`, and `JTYPE_STRING_BOOL_FALSE` return `JTYPE_STRING`. All other input integers return unchanged.

## Important Boundaries

- Preconditions and assumptions: input is an integer type code, but unknown values are allowed.
- Error/null/empty behavior: no error path; unmapped values pass through unchanged.
- Ownership and lifetime: no allocation or borrowed data.
- Concurrency/global state: no global state.
- Portability/platform constraints: none.
- Performance shape: O(1).
- Security or parsing constraints: no validation of whether the integer is a known `JTYPE_*` constant.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This helper exposes detailed-to-base type coalescing for public API users.

## Tests Observed

`test/src/JBaseItemType_test.cpp` covers boolean true/false coalescing, number-zero coalescing, all detailed string variants, and passthrough for base bool, number, string, array, and object.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:446:JBaseItemType`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers `JTYPE_NOT_PRESENT`, `JTYPE_NULL`, or unknown integer passthrough.
