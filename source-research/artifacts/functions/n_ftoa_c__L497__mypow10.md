# `mypow10`

## Status

Finalized. Source-inspected formatter power-of-ten helper.

## Identity

- Kind: function
- Definition: `n_ftoa.c:497`
- Declaration/interface: file-local prototype at `n_ftoa.c:44`
- Signature/API shape: `static JNUMBER mypow10(int exponent)`
- Visibility: file-local
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Starts from `1` and repeatedly multiplies by ten for positive exponents or divides by ten for negative exponents, returning the resulting `JNUMBER`.

## Important Boundaries

- Preconditions and assumptions: exponent ranges are bounded by formatter callers.
- Error/null/empty behavior: no error channel.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(abs(exponent)).

## Direct Call Sites

Exact source search found two `fmtflt` callers: exponent normalization and fractional-mask construction.

## Role In Callers

`fmtflt` uses `mypow10` to normalize `%g` exponent style and to scale fractional digits before rounding.

## Tests Observed

Focused JSON number handling tests exercise this indirectly through `JPrintUnformatted`.

## Graph Links

- Graph node id: `function:n_ftoa.c:497:mypow10`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
