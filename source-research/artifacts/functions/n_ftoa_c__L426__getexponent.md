# `getexponent`

## Status

Finalized. Source-inspected decimal exponent helper.

## Identity

- Kind: function
- Definition: `n_ftoa.c:426`
- Declaration/interface: file-local prototype at `n_ftoa.c:40`
- Signature/API shape: `static int getexponent(JNUMBER value)`
- Visibility: file-local
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Takes the absolute value and scales it by powers of ten until it is in `[1, 10)`, decrementing or incrementing the decimal exponent as it goes. The loops are bounded to `[-1022, 1023]`.

## Important Boundaries

- Preconditions and assumptions: `fmtflt` screens NaN/Infinity before calling.
- Error/null/empty behavior: zero returns exponent `0`.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(abs(decimal exponent)) with explicit loop bounds.

## Direct Call Sites

Exact source search found `fmtflt` as the production caller.

## Role In Callers

`fmtflt` uses `getexponent` to choose and emit `%g` exponent or fixed style.

## Tests Observed

Focused JSON number handling tests exercise this indirectly through `JPrintUnformatted`.

## Graph Links

- Graph node id: `function:n_ftoa.c:426:getexponent`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
