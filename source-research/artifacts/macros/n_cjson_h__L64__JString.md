# `JString`

## Status

Finalized. Source-inspected cJSON string type tag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:64`
- Declaration/interface: public `J.type` tag macro
- Public/exported names: `JString`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JString (1 << 4)`
- Visibility: public cJSON type constant
- Ownership/lifetime role: marks a JSON string value whose `valuestring` field carries the value

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: assigned to and compared against the low byte of `J.type`; may be combined with `JIsReference`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines the JSON string base type. String parse/create paths assign it, string-reference creation ORs it with `JIsReference`, print dispatch emits a JSON string, and `JIsString`/`JGetStringValue` classify through the low byte.

## Important Boundaries

- Preconditions and assumptions: string meaning depends on `valuestring` storage and ownership flags.
- Error/null/empty behavior: type-check helpers return false for null input; `JGetStringValue` returns only for string items.
- Ownership and lifetime: plain `JString` values own `valuestring`; `JString | JIsReference` skips `valuestring` free in `JDelete`.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit constant in the low type byte.
- Performance shape: constant-time type checks; print/delete costs depend on string length/allocation.
- Security or parsing constraints: no escaping/parsing behavior outside caller functions.

## Direct Call Sites

Source-inspected uses include `_parse_string`, `_print_value`, `JCreateString`, `JCreateStringValue`, `JCreateStringReference`, `JIsString`, `JGetStringValue`, `JType`, `JGetItemType`, and `JCompare`.

## Role In Callers

Carries string state through parsing, construction, printing, comparison, helper classification, and string getter behavior.

## Tests Observed

Focused tests cover string classification in `JType_test.cpp` and `JGetType_test.cpp`; `JAddBinaryToObject_test.cpp` exercises an allocated string-value ownership path. Source note: the `JCreateStringValue` header comment says its value string will not be freed, but the implementation sets plain `JString` without `JIsReference`.

## Graph Links

- Graph node id: `macro:n_cjson.h:64:JString`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from core parse/print/create/check functions and helper classifiers.

## Evidence Gaps

No unresolved source gaps for this node.
