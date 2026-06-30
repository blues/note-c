# `JNtoA`

## Status

Finalized. Source-inspected public `JNUMBER` to ASCII formatter.

## Identity

- Kind: function
- Definition: `n_ftoa.c:56`
- Declaration/interface: `note.h:1455`
- Signature/API shape: `char * JNtoA(JNUMBER f, char * buf, int precision)`
- Visibility: public C SDK helper
- Public/exported names: `JNtoA`

## Dependency Status

- Project-local dependencies:
  - `function:n_ftoa.c:74:fmtflt` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: `JNUMBER`, `JNTOA_PRECISION`, `JNTOA_MAX`, and formatter flag macros

## Behavior

Formats `f` into caller-provided storage and returns `buf`. Negative precision is replaced with `JNTOA_PRECISION` (`16`). The wrapper always uses `PRINT_F_TYPE_G`, passes `JNTOA_MAX` as the bounded output size to `fmtflt`, converts `fmtflt` integer-overflow reporting to the string `"0"`, writes a trailing NUL at the counted length, and returns the original buffer pointer.

## Important Boundaries

- Preconditions and assumptions: `buf` must be non-null and have at least `JNTOA_MAX` bytes as declared in `note.h`.
- Error/null/empty behavior: no null check; formatter overflow becomes `"0"`.
- Ownership and lifetime: no allocation; caller owns the buffer.
- Concurrency/global state: no global state.
- Performance shape: bounded formatter work based on precision and exponent.
- Portability/platform constraints: `JNUMBER` is `float` when `NOTE_C_SINGLE_PRECISION` is selected, otherwise `double`.

## Direct Call Sites

Exact source search found production callers in `_print_number`, `NoteSetEnvDefaultNumber`, and `NoteGetEnvNumber`, plus direct focused tests in `JNtoA_test.cpp`.

## Role In Callers

JSON printing uses `JNtoA` only for non-integral finite numbers; environment helpers use it to marshal default number values into request strings.

## Tests Observed

`JSON_number_handling_test.cpp` provides the strongest indirect output coverage through `JPrintUnformatted`. `JNtoA_test.cpp` calls `JNtoA`, but its inspected assertions do not compare or parse the generated `numStr`, so direct formatter-output evidence is weak.

## Graph Links

- Graph node id: `function:n_ftoa.c:56:JNtoA`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused direct output assertions were inspected for direct `JNtoA`, NaN/Infinity text, explicit precision values, caller-provided undersized buffers, or locale behavior.
