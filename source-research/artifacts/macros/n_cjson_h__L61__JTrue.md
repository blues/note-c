# `JTrue`

## Status

Finalized. Source-inspected cJSON true type tag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:61`
- Declaration/interface: public `J.type` tag macro
- Public/exported names: `JTrue`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTrue   (1 << 1)`
- Visibility: public cJSON type constant
- Ownership/lifetime role: marks a JSON boolean true value

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: assigned to and compared against the low byte of `J.type`; true parse path also sets `valueint = 1`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines the true boolean base type. Parse/create paths assign it for true values; print dispatch emits `true`; `JIsTrue` and `JIsBool` classify it through the low byte of `J.type`.

## Important Boundaries

- Preconditions and assumptions: boolean classification treats `JFalse` and `JTrue` as the two bool tags.
- Error/null/empty behavior: type-check helpers return false for null input.
- Ownership and lifetime: no special delete ownership behavior beyond common `JDelete`.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit constant in the low type byte.
- Performance shape: constant-time checks and switch cases.
- Security or parsing constraints: no parsing beyond the functions that assign/check the tag.

## Direct Call Sites

Source-inspected uses include `_parse_value`, `_print_value`, `JCreateTrue`, `JCreateBool`, `JIsTrue`, `JIsBool`, `JType`, `JGetItemType`, and `JCompare`.

## Role In Callers

Carries the true boolean state through parsing, construction, printing, comparison, and helper classification.

## Tests Observed

Focused tests cover true classification and bool values in `JGetType_test.cpp`, `JType_test.cpp`, and `JBoolValue_test.cpp`.

## Graph Links

- Graph node id: `macro:n_cjson.h:61:JTrue`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from core parse/print/create/check functions and helper classifiers.

## Evidence Gaps

No unresolved source gaps for this node.
