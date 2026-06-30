# `JTYPE_NUMBER_ZERO`

## Status

Finalized. Source-inspected public JSON zero-number classification constant.

## Identity

- Kind: macro
- Definition: `note.h:1405`
- Declaration/interface: public `JGetItemType`/`JBaseItemType` return code
- Public/exported names: `JTYPE_NUMBER_ZERO`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JTYPE_NUMBER_ZERO 4`
- Visibility: public note-c API constant
- Ownership/lifetime role: no ownership; classification result only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: returned for numeric items with both numeric fields zero; used by omit-empty object printing
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Represents a JSON number whose `valueint` and `valuenumber` are both zero. `JBaseItemType` maps it to `JTYPE_NUMBER`. `_last_non_omitted_object` and `_print_object` treat it as omit-empty eligible.

## Important Boundaries

- Preconditions and assumptions: classification depends on both numeric fields being zero, not only on the raw JSON spelling.
- Error/null/empty behavior: absent items return `JTYPE_NOT_PRESENT`.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: numeric zero comparison follows configured `JNUMBER` and `JINTEGER` representations.
- Performance shape: constant return/comparison after numeric checks.
- Security or parsing constraints: affects omission of zero-valued fields in omit-empty printing.

## Direct Call Sites

Exact source search found uses in `JGetItemType`, `JBaseItemType`, `_last_non_omitted_object`, `_print_object`, and focused tests.

## Role In Callers

Distinguishes zero numbers from nonzero numbers and marks zero fields as omit-empty candidates.

## Tests Observed

`JGetType_test.cpp` covers zero-number classification. `JBaseItemType_test.cpp` covers coalescing to `JTYPE_NUMBER`.

## Graph Links

- Graph node id: `macro:note.h:1405:JTYPE_NUMBER_ZERO`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JGetItemType`, `JBaseItemType`, `_last_non_omitted_object`, and `_print_object`.

## Evidence Gaps

No unresolved source gaps for this node.
