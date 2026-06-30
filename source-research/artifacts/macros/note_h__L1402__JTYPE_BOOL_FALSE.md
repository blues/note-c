# `JTYPE_BOOL_FALSE`

## Status

Finalized. Source-inspected public JSON false classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1402`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_BOOL_FALSE`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_BOOL_FALSE 2`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned for `JFalse` items; used by omit-empty object printing
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Represents a JSON item classified as boolean false. `JGetItemType` returns it for low-byte type `JFalse`; `JBaseItemType` maps it to `JTYPE_BOOL`. `_last_non_omitted_object` and `_print_object` treat this code as omit-empty eligible.

## Important Boundaries

- Preconditions and assumptions: this is a detailed helper type code, not the raw cJSON `JFalse` type tag.
- Error/null/empty behavior: false values can be omitted only through print paths using the omit-empty option.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: integer macro in public header.
- Performance shape: constant return/comparison.
- Security or parsing constraints: affects omission of fields in omit-empty printing.

## Direct Call Sites

Exact source search found uses in `JGetItemType`, `JBaseItemType`, `_last_non_omitted_object`, `_print_object`, and focused tests.

## Role In Callers

Distinguishes false booleans from true booleans and marks false fields as omit-empty candidates.

## Tests Observed

`JGetType_test.cpp` covers false classification. `JBaseItemType_test.cpp` covers coalescing to `JTYPE_BOOL`.

## Graph Links

- Graph node id: `macro:note.h:1402:JTYPE_BOOL_FALSE`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetItemType`, `JBaseItemType`, `_last_non_omitted_object`, and `_print_object`.

## Evidence Gaps

No unresolved source gaps for this node.
