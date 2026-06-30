# `JObject`

## Status

Finalized. Source-inspected cJSON object type tag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:66`
- Declaration/interface: public `J.type` tag macro
- Public/exported names: `JObject`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JObject (1 << 6)`
- Visibility: public cJSON type constant
- Ownership/lifetime role: marks a JSON object whose `child` chain carries fields

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: assigned to and compared against the low byte of `J.type`; may be combined with `JIsReference`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines the JSON object base type. Object parse/create paths assign it, object-reference creation ORs it with `JIsReference`, print/compare paths traverse child fields, and `JIsObject` classifies it through the low byte.

## Important Boundaries

- Preconditions and assumptions: object meaning depends on a valid `child` linked list and object key strings when fields are present.
- Error/null/empty behavior: type-check helpers return false for null input.
- Ownership and lifetime: plain objects own child chains; `JObject | JIsReference` skips child deletion in `JDelete`.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit constant in the low type byte.
- Performance shape: type checks are constant-time; print/compare/delete recurse over fields.
- Security or parsing constraints: parse depth is separately bounded by `N_CJSON_NESTING_LIMIT`.

## Direct Call Sites

Source-inspected uses include `_parse_object`, `_print_value`, `JCreateObject`, `JCreateObjectReference`, `JIsObject`, `JType`, `JGetItemType`, and `JCompare`.

## Role In Callers

Carries object state through parsing, construction, printing, comparison, helper classification, and ownership decisions through reference flags.

## Tests Observed

Focused tests cover object classification in `JType_test.cpp` and `JGetType_test.cpp`; several request/helper tests manually set fake return objects to `JObject`.

## Graph Links

- Graph node id: `macro:n_cjson.h:66:JObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from core parse/print/create/check functions and helper classifiers.

## Evidence Gaps

No unresolved source gaps for this node.
