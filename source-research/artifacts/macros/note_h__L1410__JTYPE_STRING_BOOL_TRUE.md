# `JTYPE_STRING_BOOL_TRUE`

## Status

Finalized. Source-inspected public string-true classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1410`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_STRING_BOOL_TRUE`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_STRING_BOOL_TRUE 9`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned when a string-like value case-insensitively equals `true`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Represents a string-like item whose `valuestring` is four characters spelling `true` case-insensitively. `JBaseItemType` maps it to `JTYPE_STRING`.

## Important Boundaries

- Preconditions and assumptions: only the exact length-four word `true` qualifies.
- Error/null/empty behavior: absent and blank strings use other codes.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: ASCII character comparisons in source.
- Performance shape: constant-length character checks after numeric parse fails.
- Security or parsing constraints: no mutation; classification only.

## Direct Call Sites

Exact source search found uses in `JGetItemType`, `JBaseItemType`, and focused tests.

## Role In Callers

Distinguishes string values that represent boolean true while preserving the base string category via `JBaseItemType`.

## Tests Observed

`JGetType_test.cpp` covers string-bool-true classification. `JBaseItemType_test.cpp` covers coalescing to `JTYPE_STRING`.

## Graph Links

- Graph node id: `macro:note.h:1410:JTYPE_STRING_BOOL_TRUE`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetItemType` and `JBaseItemType`.

## Evidence Gaps

No unresolved source gaps for this node.
