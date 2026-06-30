# `JTYPE_NULL`

## Status

Finalized. Source-inspected public JSON null classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1404`
- Declaration/interface: public `JGetItemType` return code
- Public/exported names: `JTYPE_NULL`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_NULL 3`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned for `JNULL` items
- Artifact coverage: source-inspected artifact with verified incoming constant-use edge

## Behavior

Represents a JSON item classified as null. `JGetItemType` returns it for low-byte type `JNULL`; `JBaseItemType` does not transform it.

## Important Boundaries

- Preconditions and assumptions: this is a helper return code, not a null pointer.
- Error/null/empty behavior: absent items return `JTYPE_NOT_PRESENT`, not `JTYPE_NULL`.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: integer macro in public header.
- Performance shape: constant return/comparison.
- Security or parsing constraints: no parsing; reflects helper classification only.

## Direct Call Sites

Exact source search found use in `JGetItemType`.

## Role In Callers

Allows downstream callers to distinguish explicit JSON null from missing fields.

## Tests Observed

Focused print fixtures cover null values; the emitted `JGetType_test` coverage in this repo did not include a direct `JTYPE_NULL` assertion in the inspected exact-name results.

## Graph Links

- Graph node id: `macro:note.h:1404:JTYPE_NULL`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edge from `JGetItemType`.

## Evidence Gaps

No unresolved source gaps for this node.
