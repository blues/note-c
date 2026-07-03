# `JTYPE_STRING_BOOL_FALSE`

## Status

Finalized. Source-inspected public string-false classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1411`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_STRING_BOOL_FALSE`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_STRING_BOOL_FALSE 10`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned when a string-like value case-insensitively equals `false`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Represents a string-like item whose `valuestring` is five characters spelling `false` case-insensitively. `JBaseItemType` maps it to `JTYPE_STRING`.

## Important Boundaries

- Preconditions and assumptions: only the exact length-five word `false` qualifies.
- Error/null/empty behavior: absent and blank strings use other codes.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: ASCII character comparisons in source.
- Performance shape: constant-length character checks after numeric parse fails.
- Security or parsing constraints: no mutation; classification only.

## Direct Call Sites

Exact source search found uses in `JGetItemType`, `JBaseItemType`, and focused tests.

## Role In Callers

Distinguishes string values that represent boolean false while preserving the base string category via `JBaseItemType`.

## Tests Observed

`JGetType_test.cpp` covers string-bool-false classification. `JBaseItemType_test.cpp` covers coalescing to `JTYPE_STRING`.

## Graph Links

- Graph node id: `macro:note.h:1411:JTYPE_STRING_BOOL_FALSE`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetItemType` and `JBaseItemType`.

## Evidence Gaps

No unresolved source gaps for this node.
