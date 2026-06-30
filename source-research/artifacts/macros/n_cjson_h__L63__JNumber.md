# `JNumber`

## Status

Finalized. Source-inspected cJSON number type tag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:63`
- Declaration/interface: public `J.type` tag macro
- Public/exported names: `JNumber`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JNumber (1 << 3)`
- Visibility: public cJSON type constant
- Ownership/lifetime role: marks a JSON number value whose numeric fields carry the value

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: assigned to and compared against the low byte of `J.type`; paired with `valueint` and `valuenumber`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines the JSON number base type. Number parse/create paths assign it, print dispatch sends the item through number printing, and type/helper APIs classify it as a number through `item->type & 0xFF`.

## Important Boundaries

- Preconditions and assumptions: numeric meaning also depends on `valueint` and `valuenumber` fields being populated consistently.
- Error/null/empty behavior: type-check helpers return false for null input.
- Ownership and lifetime: no heap ownership behavior tied to the tag.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit constant in the low type byte; numeric range depends on configured numeric typedefs.
- Performance shape: constant-time type checks, with printing/parsing costs in caller functions.
- Security or parsing constraints: no validation beyond parse/create paths that assign the tag.

## Direct Call Sites

Source-inspected uses include `_parse_number`, `_print_value`, `JCreateNumber`, `JCreateInteger`, `JIsNumber`, `JType`, `JGetItemType`, and `JCompare`.

## Role In Callers

Carries number state through parsing, construction, printing, comparison, and helper classification.

## Tests Observed

Focused tests cover number classification in `JGetType_test.cpp` and `JType_test.cpp`; number printing/parsing tests exercise the target paths indirectly.

## Graph Links

- Graph node id: `macro:n_cjson.h:63:JNumber`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from core parse/print/create/check functions and helper classifiers.

## Evidence Gaps

No unresolved source gaps for this node.
