# `JAtoN`

## Status

Finalized. Source-inspected locale-independent decimal string to `JNUMBER` converter.

## Identity

- Kind: function
- Definition: `n_atof.c:71`
- Declaration/interface: `note.h:1464`
- Signature/API shape: `JNUMBER JAtoN(const char *string, char **endPtr)`
- Visibility: public C SDK helper
- Public/exported names: `JAtoN`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none in the function body
- Data/type/global dependencies: `JNUMBER`, `NOTE_C_SINGLE_PRECISION`, `MAX_EXPONENT`

## Behavior

Parses an ASCII decimal number independently of locale. It skips leading space characters, handles an optional sign, scans a mantissa with at most one `.` decimal point, keeps at most 18 mantissa digits split across two integer accumulators, folds the decimal-position adjustment into the exponent, parses an optional `e`/`E` exponent with optional sign, clamps decimal exponent magnitude to `MAX_EXPONENT`, applies powers of ten by repeated binary decomposition, stores the conversion stop pointer when requested, and returns the signed `JNUMBER` result.

## Important Boundaries

- Preconditions and assumptions: `string` must be non-null; there is no null check.
- Error/null/empty behavior: if no mantissa digits are found, returns `0.0` and reports the original input pointer through `endPtr`.
- Ownership and lifetime: no allocation; returned `endPtr` points into caller-owned input.
- Concurrency/global state: no global state and no locale reads.
- Performance shape: linear over the scanned mantissa and exponent text, followed by a bounded power-of-ten loop.
- Portability/platform constraints: single-precision builds omit the largest power-of-ten table entries used for double precision.

## Direct Call Sites

Exact source search found production callers in `_parse_number`, `JGetItemType`, and `NoteGetEnvNumber`.

## Role In Callers

JSON parsing uses `JAtoN` to populate numeric nodes. `JGetItemType` uses it to classify raw/string values as numeric or zero only when the whole string is consumed. Environment helpers use it to convert string values returned by the Notecard into `JNUMBER`.

## Tests Observed

`JAtoN_test.cpp` checks approximate conversion of large and small floating-point values for both single-precision and double-precision build modes.

## Graph Links

- Graph node id: `function:n_atof.c:71:JAtoN`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused tests were inspected for malformed exponent strings, leading tabs, null input, or `endPtr` stop-position behavior.
