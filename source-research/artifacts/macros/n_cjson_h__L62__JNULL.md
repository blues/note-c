# `JNULL`

## Status

Finalized. Source-inspected cJSON null type tag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:62`
- Declaration/interface: public `J.type` tag macro
- Public/exported names: `JNULL`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JNULL   (1 << 2)`
- Visibility: public cJSON type constant
- Ownership/lifetime role: marks a JSON null value

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: assigned to and compared against the low byte of `J.type`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines the JSON null base type. The parser assigns it for `null`, print dispatch emits `null`, and `JIsNull` checks it through the low byte of `J.type`.

## Important Boundaries

- Preconditions and assumptions: `JNULL` is a type tag, not a null pointer sentinel.
- Error/null/empty behavior: `JIsNull` returns false for null input.
- Ownership and lifetime: no special delete ownership behavior beyond common `JDelete`.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit constant in the low type byte.
- Performance shape: constant-time checks and switch cases.
- Security or parsing constraints: no parser behavior beyond assignment/checking of the tag.

## Direct Call Sites

Source-inspected uses include `_parse_value`, `_print_value`, `JIsNull`, `JType`, `JGetItemType`, and `JCompare`.

## Role In Callers

Carries null state through parsing, printing, comparison, and helper classification.

## Tests Observed

Focused print tests cover null through parse/print fixtures. Gap: no `JCreateNull` or `JAddNull` production API was found.

## Graph Links

- Graph node id: `macro:n_cjson.h:62:JNULL`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from core parse/print/check functions and helper classifiers.

## Evidence Gaps

No unresolved source gaps for this node.
