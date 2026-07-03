# `JTYPE_NUMBER`

## Status

Finalized. Source-inspected public JSON nonzero/base number classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1406`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_NUMBER`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_NUMBER 5`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned for nonzero numeric items and by base-type coalescing
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Represents a JSON number that is not classified as zero, and the base class for all numbers returned by `JBaseItemType`.

## Important Boundaries

- Preconditions and assumptions: detailed classification distinguishes `JTYPE_NUMBER_ZERO` before returning this code.
- Error/null/empty behavior: absent items return `JTYPE_NOT_PRESENT`.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: integer macro in public header.
- Performance shape: constant return/comparison.
- Security or parsing constraints: no parsing; reflects helper classification only.

## Direct Call Sites

Exact source search found uses in `JGetItemType`, `JBaseItemType`, and focused tests.

## Role In Callers

Provides both detailed nonzero-number classification and base number category for coalesced type handling.

## Tests Observed

`JGetType_test.cpp` covers nonzero number classification. `JBaseItemType_test.cpp` covers zero-number coalescing and passthrough of `JTYPE_NUMBER`.

## Graph Links

- Graph node id: `macro:note.h:1406:JTYPE_NUMBER`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetItemType` and `JBaseItemType`.

## Evidence Gaps

No unresolved source gaps for this node.
