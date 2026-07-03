# `JFalse`

## Status

Finalized. Source-inspected cJSON false type tag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:60`
- Declaration/interface: public `J.type` tag macro
- Public/exported names: `JFalse`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JFalse  (1 << 0)`
- Visibility: public cJSON type constant
- Ownership/lifetime role: marks a JSON boolean false value

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: assigned to and compared against the low byte of `J.type`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines the false boolean base type. Parse/create paths assign it for false values; print dispatch emits `false`; `JIsFalse` and `JIsBool` classify it through the low byte of `J.type`.

## Important Boundaries

- Preconditions and assumptions: boolean classification treats `JFalse` and `JTrue` as the two bool tags.
- Error/null/empty behavior: type-check helpers return false for null input.
- Ownership and lifetime: no special delete ownership behavior beyond common `JDelete`.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit constant in the low type byte.
- Performance shape: constant-time checks and switch cases.
- Security or parsing constraints: no parsing beyond the functions that assign/check the tag.

## Direct Call Sites

Source-inspected uses include `_parse_value`, `_print_value`, `JCreateFalse`, `JCreateBool`, `JIsFalse`, `JIsBool`, `JType`, `JGetItemType`, and `JCompare`.

## Role In Callers

Carries the false boolean state through parsing, construction, printing, comparison, and helper classification.

## Tests Observed

Focused tests cover false classification and bool values in `JGetType_test.cpp`, `JType_test.cpp`, and `JBoolValue_test.cpp`.

## Graph Links

- Graph node id: `macro:n_cjson.h:60:JFalse`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from core parse/print/create/check functions and helper classifiers.

## Evidence Gaps

No unresolved source gaps for this node.
