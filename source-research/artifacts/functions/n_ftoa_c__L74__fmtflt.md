# `fmtflt`

## Status

Finalized. Source-inspected constrained floating-point formatter used by `JNtoA`.

## Identity

- Kind: function
- Definition: `n_ftoa.c:74`
- Declaration/interface: file-local prototype at `n_ftoa.c:37`
- Signature/API shape: `static void fmtflt(char *str, size_t *len, size_t size, JNUMBER fvalue, int width, int precision, int flags, int *overflow)`
- Visibility: file-local
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_ftoa.c:378:fmtstr` finalized
  - `function:n_ftoa.c:426:getexponent` finalized
  - `function:n_ftoa.c:497:mypow10` finalized
  - `function:n_ftoa.c:462:cast` finalized
  - `function:n_ftoa.c:486:myround` finalized
  - `function:n_ftoa.c:448:convert` finalized
  - `function:n_ftoa.c:420:getnumsep` finalized
  - `macro:n_ftoa.c:45:OUTCHAR` finalized
  - `function:n_ftoa.c:415:printsep` finalized
- External/system dependencies: `isnan`, `isinf`, `UINTMAX_MAX`
- Data/type/global dependencies: writes caller-provided output buffer, logical length counter, and overflow flag

## Behavior

Implements the `%g` path used by `JNtoA`, plus dormant support for some other formatting flags. It handles sign selection, direct NaN/Infinity strings through `fmtstr`, exponent calculation for `%g`, precision clamping based on `sizeof(uintmax_t)`, integer/fraction splitting, half-up fractional rounding, carry into the integer and exponent, `%g` fixed-vs-exponent style selection, optional omission of trailing fractional zeros, padding, optional grouping separators, and output emission through `OUTCHAR`.

## Important Boundaries

- Preconditions and assumptions: caller provides valid output state and a valid overflow pointer.
- Error/null/empty behavior: sets `*overflow = 1` and returns when `cast` reports integer overflow.
- Ownership and lifetime: uses stack scratch buffers only; no allocation.
- Concurrency/global state: no global state.
- Performance shape: bounded by decimal exponent, precision, and emitted output length.
- Portability/platform constraints: precision is clamped to 9, 19, or 38 digits depending on `uintmax_t` width; `cast` handles unusual floating-to-integer conversion behavior.

## Direct Call Sites

Exact source search found `JNtoA` as the production caller.

## Role In Callers

`JNtoA` calls `fmtflt` with `PRINT_F_TYPE_G`, `JNTOA_MAX`, no minimum width, and default or caller-supplied precision.

## Tests Observed

Focused JSON number handling tests exercise this through `JPrintUnformatted`. Direct `JNtoA` tests call the function but currently do not assert `numStr` output, so they are weak evidence.

## Graph Links

- Graph node id: `function:n_ftoa.c:74:fmtflt`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused tests were inspected for direct `JNtoA` NaN/Infinity text, explicit precision values, grouping flags, or unusual 128-bit `uintmax_t` precision boundaries.
