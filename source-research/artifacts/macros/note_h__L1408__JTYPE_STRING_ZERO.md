# `JTYPE_STRING_ZERO`

## Status

Finalized. Source-inspected public string-zero classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1408`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_STRING_ZERO`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_STRING_ZERO 7`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned when a string-like value parses numerically to zero
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Represents a string-like item whose `valuestring` parses completely as a numeric value equal to zero. `JBaseItemType` maps it to `JTYPE_STRING`.

## Important Boundaries

- Preconditions and assumptions: classification uses `JAtoN` and requires the whole string to parse as a number.
- Error/null/empty behavior: null or empty strings are classified as `JTYPE_STRING_BLANK`, not this code.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: numeric parsing follows configured number conversion behavior.
- Performance shape: requires string length scan and numeric parse.
- Security or parsing constraints: no mutation; classification only.

## Direct Call Sites

Exact source search found uses in `JGetItemType`, `JBaseItemType`, and focused tests.

## Role In Callers

Distinguishes string values that represent zero from other string-like values before base-type coalescing.

## Tests Observed

`JGetType_test.cpp` covers string-zero classification. `JBaseItemType_test.cpp` covers coalescing to `JTYPE_STRING`.

## Graph Links

- Graph node id: `macro:note.h:1408:JTYPE_STRING_ZERO`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetItemType` and `JBaseItemType`.

## Evidence Gaps

No unresolved source gaps for this node.
