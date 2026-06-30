# `JStringValue`

## Status

Finalized. Source-inspected JSON item string-value accessor, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:83`
- Declaration/interface: `note.h:1294`
- Public/exported names: `JStringValue`
- Macro aliases/wrappers: none
- Signature/API shape: `char *JStringValue(J *item)`
- Visibility: public note-c helper API
- Ownership/lifetime role: returns borrowed `valuestring` pointer from caller-owned item

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `J.valuestring`
- Artifact coverage: source, declaration, direct caller search, and focused tests inspected

## Behavior

Returns a pointer to the literal empty string when `item` is `NULL`. For non-null input, returns `item->valuestring` directly without checking the item type and without replacing a null `valuestring`.

## Important Boundaries

- Preconditions and assumptions: non-null input must point to a valid `J`.
- Error/null/empty behavior: null item returns `""`; non-null item with null `valuestring` returns `NULL`.
- Ownership and lifetime: returned pointer is borrowed from the item, except the null-item fallback points to static string storage.
- Concurrency/global state: no mutable global state.
- Portability/platform constraints: none.
- Performance shape: O(1).
- Security or parsing constraints: this is a raw field accessor, not a string-type validator.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This helper exposes raw item string extraction for users that already hold a `J *` item.

## Tests Observed

`test/src/JStringValue_test.cpp` covers null item returning `""`, number item returning `NULL`, and valid string item returning the stored string.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:83:JStringValue`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test forces a string-typed item to have `valuestring == NULL`. No production caller was found.
