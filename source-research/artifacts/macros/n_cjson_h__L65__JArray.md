# `JArray`

## Status

Finalized. Source-inspected cJSON array type tag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:65`
- Declaration/interface: public `J.type` tag macro
- Public/exported names: `JArray`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JArray  (1 << 5)`
- Visibility: public cJSON type constant
- Ownership/lifetime role: marks a JSON array whose `child` chain carries elements

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: assigned to and compared against the low byte of `J.type`; may be combined with `JIsReference`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines the JSON array base type. Array parse/create paths assign it, array-reference creation ORs it with `JIsReference`, print/compare paths recurse over child elements, and `JIsArray` classifies it through the low byte.

## Important Boundaries

- Preconditions and assumptions: array meaning depends on a valid `child` linked list when elements are present.
- Error/null/empty behavior: type-check helpers return false for null input.
- Ownership and lifetime: plain arrays own child chains; `JArray | JIsReference` skips child deletion in `JDelete`.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit constant in the low type byte.
- Performance shape: type checks are constant-time; print/compare/delete recurse over children.
- Security or parsing constraints: parse depth is separately bounded by `N_CJSON_NESTING_LIMIT`.

## Direct Call Sites

Source-inspected uses include `_parse_array`, `_print_value`, `JCreateArray`, `JCreateArrayReference`, `JIsArray`, `JType`, `JGetItemType`, and `JCompare`.

## Role In Callers

Carries array state through parsing, construction, printing, comparison, helper classification, and ownership decisions through reference flags.

## Tests Observed

Focused tests cover array classification in `JType_test.cpp` and `JGetType_test.cpp`.

## Graph Links

- Graph node id: `macro:n_cjson.h:65:JArray`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from core parse/print/create/check functions and helper classifiers.

## Evidence Gaps

No unresolved source gaps for this node.
