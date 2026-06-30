# `JTYPE_STRING_BLANK`

## Status

Finalized. Source-inspected public blank-string classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1407`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_STRING_BLANK`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_STRING_BLANK 6`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned for string/raw items with null or empty `valuestring`; used by omit-empty object printing
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Represents a string-like item whose `valuestring` is null or empty. `JBaseItemType` maps it to `JTYPE_STRING`. `_last_non_omitted_object` and `_print_object` treat it as omit-empty eligible.

## Important Boundaries

- Preconditions and assumptions: applies to `JString` and `JRaw` low-byte types handled by `JGetItemType`.
- Error/null/empty behavior: absent items return `JTYPE_NOT_PRESENT`; empty string values return this code.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: integer macro in public header.
- Performance shape: constant return/comparison after first-character check.
- Security or parsing constraints: affects omission of blank-string fields in omit-empty printing.

## Direct Call Sites

Exact source search found uses in `JGetItemType`, `JBaseItemType`, `_last_non_omitted_object`, `_print_object`, and focused tests.

## Role In Callers

Distinguishes blank strings from numeric/bool-like strings and marks blank string fields as omit-empty candidates.

## Tests Observed

`JGetType_test.cpp` covers blank-string classification. `JBaseItemType_test.cpp` covers coalescing to `JTYPE_STRING`.

## Graph Links

- Graph node id: `macro:note.h:1407:JTYPE_STRING_BLANK`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetItemType`, `JBaseItemType`, `_last_non_omitted_object`, and `_print_object`.

## Evidence Gaps

No unresolved source gaps for this node.
