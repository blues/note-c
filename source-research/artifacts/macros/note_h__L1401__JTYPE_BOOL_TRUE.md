# `JTYPE_BOOL_TRUE`

## Status

Finalized. Source-inspected public JSON true classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1401`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_BOOL_TRUE`
- Macro aliases/wrappers: target of `JTYPE_BOOL`
- Signature/API shape: `#define JTYPE_BOOL_TRUE 1`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned for `JTrue` items and coalesced by `JBaseItemType`
- Artifact coverage: source-inspected artifact with verified incoming constant-use and alias edges

## Behavior

Represents a JSON item classified as boolean true. `JGetItemType` returns it for low-byte type `JTrue`; `JBaseItemType` maps it to `JTYPE_BOOL`. `JTYPE_BOOL` is an alias for this value.

## Important Boundaries

- Preconditions and assumptions: this is a detailed helper type code, not the raw cJSON `JTrue` type tag.
- Error/null/empty behavior: not used for null items.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: integer macro in public header.
- Performance shape: constant return/comparison.
- Security or parsing constraints: no parsing; reflects helper classification only.

## Direct Call Sites

Exact source search found uses in `JGetItemType`, `JBaseItemType`, `JTYPE_BOOL`, and focused tests.

## Role In Callers

Allows callers to distinguish true booleans from false booleans before optional base-type coalescing.

## Tests Observed

`JGetType_test.cpp` covers true classification. `JBaseItemType_test.cpp` covers coalescing to `JTYPE_BOOL`.

## Graph Links

- Graph node id: `macro:note.h:1401:JTYPE_BOOL_TRUE`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetItemType` and `JBaseItemType`, plus verified `JTYPE_BOOL` alias edge.

## Evidence Gaps

No unresolved source gaps for this node.
