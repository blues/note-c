# `JGetItemType`

## Status

Finalized. Source-inspected detailed JSON item type classifier.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:387`
- Declaration/interface: `note.h:1431`
- Signature/API shape: `int JGetItemType(J *item)`
- Visibility: public C SDK helper
- Public/exported names: `JGetItemType`

## Dependency Status

- Project-local dependencies:
  - `function:n_atof.c:71:JAtoN` finalized
- External/system dependencies: `strlen`
- Data/type/global dependencies: `J.type`, `J.valueint`, `J.valuenumber`, `J.valuestring`, `JTYPE_*` constants

## Behavior

Returns `JTYPE_NOT_PRESENT` for a null item or an unknown base type. Boolean, null, object, and array nodes map directly to their detailed type constants. Number nodes are classified as `JTYPE_NUMBER_ZERO` only when both `valueint` and `valuenumber` are zero; otherwise they are `JTYPE_NUMBER`. String and raw nodes with null or empty `valuestring` are `JTYPE_STRING_BLANK`; otherwise the helper calls `JAtoN` and classifies the value as `JTYPE_STRING_ZERO` or `JTYPE_STRING_NUMBER` only when the conversion consumes the full string. Remaining string values are checked case-insensitively for literal `true` or `false` before falling back to `JTYPE_STRING`.

## Important Boundaries

- Preconditions and assumptions: accepts null item; string classification assumes `valuestring` is NUL-terminated when non-null.
- Error/null/empty behavior: null item and invalid base type return `JTYPE_NOT_PRESENT`.
- Ownership and lifetime: no allocation; reads fields from caller-owned `J`.
- Concurrency/global state: no global state.
- Performance shape: O(length of string/raw value) for string-like nodes.
- Portability/platform constraints: numeric string classification follows `JAtoN` and therefore `JNUMBER` precision.

## Direct Call Sites

Exact source search found direct production callers in `JGetType`, `_last_non_omitted_object`, and `_print_object`.

## Role In Callers

`JGetType` exposes field-type classification. The object printer uses `JGetItemType` for omit-empty decisions and for look-ahead that suppresses trailing commas after omitted fields.

## Tests Observed

`JGetType_test.cpp` exercises this through `JGetType` for null/missing fields, booleans, number zero/nonzero, blank strings, numeric strings, boolean strings, objects, arrays, and invalid nodes.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:387:JGetItemType`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test was inspected for direct `JGetItemType` calls, mixed numeric suffix strings, leading-space numeric strings, or omit-empty printing through this helper.
