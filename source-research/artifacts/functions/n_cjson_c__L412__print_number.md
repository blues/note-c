# `_print_number`

## Status

Finalized. Source-inspected JSON number serializer.

## Identity

- Kind: function
- Definition: `n_cjson.c:412`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `Jbool _print_number(const J * const item, printbuffer * const output_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:229:_get_decimal_point` finalized
  - `function:n_str.c:31:strlcpy` finalized
  - `function:n_ftoa.c:56:JNtoA` finalized
  - `function:n_cjson_helpers.c:284:JItoA` finalized
  - `function:n_cjson.c:351:_ensure` finalized
- External/system dependencies: `strlen`
- Data/type/global dependencies: reads `J.valuenumber`, `J.valueint`, `JNTOA_MAX`, and locale decimal point in locale-enabled builds

## Behavior

Rejects null item or output buffer. It prints non-finite values as JSON `null` using `strlcpy`; finite non-integral values use `JNtoA(vnum, nbuf, -1)`, and finite integral values use `JItoA(vint, nbuf)`. It checks the temporary buffer length against `JNTOA_MAX - 1`, reserves output through `_ensure`, copies the bytes into the printbuffer while replacing the active locale decimal separator with `.`, NUL-terminates the fragment, advances `output_buffer->offset`, and returns true.

## Important Boundaries

- Preconditions and assumptions: `item` is a number node and `output_buffer` owns valid printbuffer storage.
- Error/null/empty behavior: null inputs, overlong converted text, or `_ensure` failure return false.
- Ownership and lifetime: no standalone allocation; writes into printbuffer storage.
- Concurrency/global state: locale-enabled builds read process locale through `_get_decimal_point`.
- Performance shape: bounded by `JNTOA_MAX`.
- Portability/platform constraints: finite floating formatting follows `JNtoA`, whose precision depends on `JNUMBER` width.

## Direct Call Sites

Exact source search found `_print_value` as the production caller for `JNumber` values.

## Role In Callers

`_print_value` delegates JSON number values here for all public print entry points, including `JPrintUnformatted`.

## Tests Observed

`JSON_number_handling_test.cpp` has focused JSON marshalling assertions for zero, integer limits, out-of-range values, small floating-point output, and timestamp-style values. No locale-specific decimal separator test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:412:_print_number`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused tests were inspected for JSON NaN/Infinity-to-`null`, locale decimal replacement, or allocation failure in this exact helper.
