# `myround`

## Status

Finalized. Source-inspected formatter rounding helper.

## Identity

- Kind: function
- Definition: `n_ftoa.c:486`
- Declaration/interface: file-local prototype at `n_ftoa.c:43`
- Signature/API shape: `static uintmax_t myround(JNUMBER value)`
- Visibility: file-local
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_ftoa.c:462:cast` finalized
- External/system dependencies: `UINTMAX_MAX`
- Data/type/global dependencies: none

## Behavior

Converts the integer portion with `cast`; if that reports overflow, returns `UINTMAX_MAX`. Otherwise subtracts the integer portion and returns `intpart` for fractions below `0.5`, or `intpart + 1` for fractions at least `0.5`.

## Important Boundaries

- Preconditions and assumptions: caller supplies a finite non-negative value.
- Error/null/empty behavior: propagates `UINTMAX_MAX` overflow sentinel.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(1).

## Direct Call Sites

Exact source search found `fmtflt` as the production caller.

## Role In Callers

`fmtflt` uses `myround` to round the scaled fractional part, including carry into the integer/exponent path.

## Tests Observed

Focused JSON number handling tests exercise rounding indirectly through `JPrintUnformatted`.

## Graph Links

- Graph node id: `function:n_ftoa.c:486:myround`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
