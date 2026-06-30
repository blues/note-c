# `JTYPE_BOOL`

## Status

Finalized. Source-inspected public JSON boolean base classification alias.

## Identity

- Kind: macro
- Definition: `note.h:1403`
- Declaration/interface: public base type return code
- Public/exported names: `JTYPE_BOOL`
- Macro aliases/wrappers: aliases `JTYPE_BOOL_TRUE`
- Signature/API shape: `#define JTYPE_BOOL JTYPE_BOOL_TRUE`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies:
  - `macro:note.h:1401:JTYPE_BOOL_TRUE` finalized
- External/system dependencies: none
- Data/type/global dependencies: returned by `JBaseItemType`
- Artifact coverage: source-inspected artifact with verified macro-alias and constant-use edges

## Behavior

Aliases the base boolean class to the true value's numeric code. `JBaseItemType` maps both `JTYPE_BOOL_TRUE` and `JTYPE_BOOL_FALSE` to this code.

## Important Boundaries

- Preconditions and assumptions: callers using detailed type codes must not assume `JTYPE_BOOL` distinguishes true from false.
- Error/null/empty behavior: not used for missing values.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: macro alias in public header.
- Performance shape: compile-time alias.
- Security or parsing constraints: no parsing; base-class coalescing only.

## Direct Call Sites

Exact source search found uses in `JBaseItemType` and focused tests.

## Role In Callers

Provides a common bool category for callers that do not care about true versus false.

## Tests Observed

`JBaseItemType_test.cpp` covers true/false coalescing and passthrough of `JTYPE_BOOL`.

## Graph Links

- Graph node id: `macro:note.h:1403:JTYPE_BOOL`
- Structured graph: `inventory/dependency-graph.json`
- Verified `expands_to` edge to `JTYPE_BOOL_TRUE` and verified incoming `uses_constant` edge from `JBaseItemType`.

## Evidence Gaps

No unresolved source gaps for this node.
