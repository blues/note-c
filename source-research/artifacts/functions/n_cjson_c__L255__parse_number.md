# `_parse_number`

## Status

Finalized. Source-inspected JSON number parser.

## Identity

- Kind: function
- Definition: `n_cjson.c:255`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `Jbool _parse_number(J * const item, parse_buffer * const input_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:229:_get_decimal_point` finalized
  - `macro:n_cjson.c:249:can_access_at_index` finalized
  - `macro:n_cjson.c:252:buffer_at_offset` finalized
  - `function:n_atof.c:71:JAtoN` finalized
  - `function:n_cjson_helpers.c:312:JAtoI` finalized
- External/system dependencies: none
- Data/type/global dependencies: `J.valuenumber`, `J.valueint`, `J.type`, `JINTEGER_MIN`, `JINTEGER_MAX`, `JNumber`

## Behavior

Rejects null input buffers or null content. It copies a bounded run of numeric characters into a 64-byte temporary string, translating JSON `.` to the active decimal separator. It parses the temporary string with `JAtoN`, rejects the case where no characters were consumed, stores `valuenumber`, saturates `valueint` at `JINTEGER_MAX`/`JINTEGER_MIN` when needed, otherwise parses the integer value with `JAtoI`, sets type to `JNumber`, advances the parse-buffer offset by the consumed length, and returns true.

## Important Boundaries

- Preconditions and assumptions: caller supplies a valid `item` and starts at a candidate number byte.
- Error/null/empty behavior: no consumed numeric characters or invalid input buffer returns false.
- Ownership and lifetime: no allocation; writes into caller-owned `J`.
- Concurrency/global state: locale-enabled builds read process locale through `_get_decimal_point`.
- Performance shape: bounded by the 63-byte temporary numeric token buffer.
- Portability/platform constraints: numeric precision follows `JNUMBER`; integer saturation follows `JINTEGER`.

## Direct Call Sites

Exact source search found `_parse_value` as the production caller for numeric JSON values.

## Role In Callers

The root parser and recursive array/object parsers reach this helper through `_parse_value`.

## Tests Observed

`JSON_number_handling_test.cpp` covers parse behavior for zero, integer bounds/saturation, floating values, small values, and timestamp-like numbers.

## Graph Links

- Graph node id: `function:n_cjson.c:255:_parse_number`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused tests were identified for locale-specific decimal parsing or numeric tokens longer than the temporary buffer.
