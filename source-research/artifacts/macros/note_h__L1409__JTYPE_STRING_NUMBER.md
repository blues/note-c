# `JTYPE_STRING_NUMBER`

## Status

Finalized. Source-inspected public string-number classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1409`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_STRING_NUMBER`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_STRING_NUMBER 8`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned when a string-like value parses completely as a nonzero number
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Represents a string-like item whose `valuestring` parses completely as a numeric value other than zero. `JBaseItemType` maps it to `JTYPE_STRING`.

## Important Boundaries

- Preconditions and assumptions: classification uses `JAtoN` and requires the whole string to parse as a number.
- Error/null/empty behavior: empty strings are classified as blank; numeric zero strings use `JTYPE_STRING_ZERO`.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: numeric parsing follows configured number conversion behavior.
- Performance shape: requires string length scan and numeric parse.
- Security or parsing constraints: no mutation; classification only.

## Direct Call Sites

Exact source search found uses in `JGetItemType`, `JBaseItemType`, and focused tests.

## Role In Callers

Distinguishes string values that represent nonzero numbers from ordinary strings before base-type coalescing.

## Tests Observed

`JGetType_test.cpp` covers string-number classification. `JBaseItemType_test.cpp` covers coalescing to `JTYPE_STRING`.

## Graph Links

- Graph node id: `macro:note.h:1409:JTYPE_STRING_NUMBER`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetItemType` and `JBaseItemType`.

## Evidence Gaps

No unresolved source gaps for this node.
