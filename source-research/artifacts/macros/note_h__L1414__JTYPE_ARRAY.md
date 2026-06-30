# `JTYPE_ARRAY`

## Status

Finalized. Source-inspected public JSON array classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1414`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_ARRAY`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_ARRAY 13`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned for `JArray` items
- Artifact coverage: source-inspected artifact with verified incoming constant-use edge

## Behavior

Represents a JSON array item. `JGetItemType` returns it for low-byte type `JArray`; `JBaseItemType` leaves it unchanged.

## Important Boundaries

- Preconditions and assumptions: this is a helper return code, not the raw cJSON `JArray` type tag.
- Error/null/empty behavior: absent items return `JTYPE_NOT_PRESENT`.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: integer macro in public header.
- Performance shape: constant return/comparison.
- Security or parsing constraints: no traversal; classification only.

## Direct Call Sites

Exact source search found uses in `JGetItemType` and focused tests.

## Role In Callers

Allows downstream callers to switch on array fields without inspecting `J.type` directly.

## Tests Observed

`JGetType_test.cpp` covers array classification. `JBaseItemType_test.cpp` covers passthrough of `JTYPE_ARRAY`.

## Graph Links

- Graph node id: `macro:note.h:1414:JTYPE_ARRAY`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edge from `JGetItemType`.

## Evidence Gaps

No unresolved source gaps for this node.
