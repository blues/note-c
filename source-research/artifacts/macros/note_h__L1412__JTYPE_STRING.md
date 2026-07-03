# `JTYPE_STRING`

## Status

Finalized. Source-inspected public JSON string/base string classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1412`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_STRING`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_STRING 11`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned for ordinary string-like values and by base-type coalescing
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Represents an ordinary string-like item that is not blank, numeric, or boolean-like, and the base string category returned by `JBaseItemType`.

## Important Boundaries

- Preconditions and assumptions: `JGetItemType` checks blank, numeric, and bool-like string cases before returning this code.
- Error/null/empty behavior: absent items return `JTYPE_NOT_PRESENT`; blank strings return `JTYPE_STRING_BLANK`.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: integer macro in public header.
- Performance shape: constant return/comparison after classification checks.
- Security or parsing constraints: no mutation; classification only.

## Direct Call Sites

Exact source search found uses in `JGetItemType`, `JBaseItemType`, and focused tests.

## Role In Callers

Provides detailed ordinary-string classification and the base string category for all string-like subtype codes.

## Tests Observed

`JGetType_test.cpp` covers string classification. `JBaseItemType_test.cpp` covers coalescing and passthrough of `JTYPE_STRING`.

## Graph Links

- Graph node id: `macro:note.h:1412:JTYPE_STRING`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetItemType` and `JBaseItemType`.

## Evidence Gaps

No unresolved source gaps for this node.
