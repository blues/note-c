# `_last_non_omitted_object`

## Status

Finalized. Source-inspected object-printer look-ahead helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1507`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `bool _last_non_omitted_object(J * item, printbuffer * const output_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson_helpers.c:387:JGetItemType` finalized
- External/system dependencies: none
- Data/type/global dependencies: `J.next`, `printbuffer.omitempty`, omit-empty `JTYPE_*` constants

## Behavior

When omit-empty is disabled, returns whether the current object member has no `next` sibling. When omit-empty is enabled, walks following siblings and returns false as soon as a later field would be printed. Fields classified as `JTYPE_BOOL_FALSE`, `JTYPE_NUMBER_ZERO`, or `JTYPE_STRING_BLANK` are skipped by the look-ahead; any other later type means the current field is not the last non-omitted field.

## Important Boundaries

- Preconditions and assumptions: caller supplies non-null `item` and `output_buffer`.
- Error/null/empty behavior: no null checks and no error channel.
- Ownership and lifetime: no allocation; traverses the caller-owned object child list.
- Concurrency/global state: no global state.
- Performance shape: O(number of following object siblings) when omit-empty is enabled.
- Portability/platform constraints: omit decisions follow `JGetItemType` numeric/string classification.

## Direct Call Sites

Exact source search found `_print_object` as the sole production caller.

## Role In Callers

`_print_object` uses this helper after printing a field to decide whether to emit a comma, avoiding trailing commas when later siblings are omitted.

## Tests Observed

No direct omit-empty tests were found in `test/src`. `JGetType_test.cpp` covers the `JGetItemType` classifications this helper relies on.

## Graph Links

- Graph node id: `function:n_cjson.c:1507:_last_non_omitted_object`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused omit-empty print test was inspected for comma suppression across omitted fields.
