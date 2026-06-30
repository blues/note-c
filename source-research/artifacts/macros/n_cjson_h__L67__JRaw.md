# `JRaw`

## Status

Finalized. Source-inspected cJSON raw JSON type tag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:67`
- Declaration/interface: public `J.type` tag macro
- Public/exported names: `JRaw`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JRaw    (1 << 7)`
- Visibility: public cJSON type constant
- Ownership/lifetime role: marks a raw JSON string payload

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: assigned to and compared against the low byte of `J.type`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines the raw JSON base type. `JCreateRaw` assigns it; print dispatch copies `valuestring` without normal JSON string escaping; `JIsRaw` checks it through the low byte. Helper classification treats raw as string-like.

## Important Boundaries

- Preconditions and assumptions: raw values should contain already-valid JSON text when printed.
- Error/null/empty behavior: type-check helpers return false for null input.
- Ownership and lifetime: plain raw values own `valuestring`; no parser path creates `JRaw`.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit constant in the low type byte.
- Performance shape: constant-time checks; printing cost depends on raw string length.
- Security or parsing constraints: raw output bypasses string escaping, so caller-provided raw text controls emitted JSON fragment.

## Direct Call Sites

Source-inspected uses include `_print_value`, `JCreateRaw`, `JIsRaw`, `JType`, `JGetItemType`, and `JCompare`.

## Role In Callers

Carries raw JSON state through construction, printing, comparison, and helper classification.

## Tests Observed

Focused tests cover raw classification in `JType_test.cpp` and `JGetType_test.cpp`. Gap: no parser path creates `JRaw`.

## Graph Links

- Graph node id: `macro:n_cjson.h:67:JRaw`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from core print/create/check functions and helper classifiers.

## Evidence Gaps

No unresolved source gaps for this node.
