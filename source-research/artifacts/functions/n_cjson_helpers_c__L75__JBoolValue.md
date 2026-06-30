# `JBoolValue`

## Status

Finalized. Source-inspected JSON item boolean-value accessor, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:75`
- Declaration/interface: `note.h:1302`
- Public/exported names: `JBoolValue`
- Macro aliases/wrappers: none
- Signature/API shape: `bool JBoolValue(J *item)`
- Visibility: public note-c helper API
- Ownership/lifetime role: borrowed read of caller-owned item

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `J.type`
- Artifact coverage: source, declaration, direct caller search, and focused tests inspected

## Behavior

Returns `false` when `item` is `NULL`. Otherwise masks `item->type` with `0xff` and returns true only when the base type is exactly `JTrue`.

## Important Boundaries

- Preconditions and assumptions: non-null input must point to a valid `J`.
- Error/null/empty behavior: null, false, and non-true types all return `false`.
- Ownership and lifetime: no allocation; reads caller-owned item.
- Concurrency/global state: no global state.
- Portability/platform constraints: none.
- Performance shape: O(1).
- Security or parsing constraints: exact true base-type check after masking flags.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This helper exposes scalar bool extraction for users that already hold a `J *` item.

## Tests Observed

`test/src/JBoolValue_test.cpp` covers null item, false item, true item, and non-bool number item.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:75:JBoolValue`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers malformed or mixed boolean type bits. No production caller was found.
