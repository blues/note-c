# `JTYPE_NOT_PRESENT`

## Status

Finalized. Source-inspected public JSON type-classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1400`
- Declaration/interface: public `JGetType`/`JGetItemType` return code
- Public/exported names: `JTYPE_NOT_PRESENT`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_NOT_PRESENT 0`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned by `JGetType` and `JGetItemType`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Represents absent or unclassifiable JSON content. `JGetType` returns it for null object or null field arguments, and `JGetItemType` returns it for null items or base types that do not match a handled `J.type`.

## Important Boundaries

- Preconditions and assumptions: callers treat this as a return code, not a `J.type` low-byte tag.
- Error/null/empty behavior: null input maps to this code.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: integer macro in public header.
- Performance shape: constant return/comparison.
- Security or parsing constraints: no parsing; reflects helper classification only.

## Direct Call Sites

Exact source search found uses in `JGetType`, `JGetItemType`, and the public header declaration comments.

## Role In Callers

Allows downstream callers to distinguish missing/unclassifiable fields from concrete JSON type categories.

## Tests Observed

`test/src/JGetType_test.cpp` covers null JSON, null field, missing field, and invalid type returning `JTYPE_NOT_PRESENT`.

## Graph Links

- Graph node id: `macro:note.h:1400:JTYPE_NOT_PRESENT`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetType` and `JGetItemType`.

## Evidence Gaps

No unresolved source gaps for this node.
