# `JGetItemName`

## Status

Finalized. Source-inspected JSON item name accessor, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:276`
- Declaration/interface: `note.h:1379`
- Public/exported names: `JGetItemName`
- Macro aliases/wrappers: none
- Signature/API shape: `const char *JGetItemName(const J * item)`
- Visibility: public note-c helper API
- Ownership/lifetime role: returns borrowed item key pointer or static empty string

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `J.string`
- Artifact coverage: source, declaration, caller search, and focused tests inspected

## Behavior

Returns `""` when `item` is `NULL` or `item->string` is `NULL`; otherwise returns `item->string`.

## Important Boundaries

- Preconditions and assumptions: non-null input must point to a valid `J`.
- Error/null/empty behavior: null item and unnamed item both return the empty string.
- Ownership and lifetime: non-empty return aliases the JSON item key and must not be freed by the caller or retained past item/tree deletion.
- Concurrency/global state: no mutable global state.
- Portability/platform constraints: none.
- Performance shape: O(1).
- Security or parsing constraints: no validation or copying of the key string.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This helper exposes item key access for public API users.

## Tests Observed

`test/src/JGetItemName_test.cpp` covers null item, unnamed object item, and a valid named object child.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:276:JGetItemName`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers lifetime/aliasing behavior or const-key items.
