# `getnumsep`

## Status

Finalized. Source-inspected grouping-separator counter.

## Identity

- Kind: function
- Definition: `n_ftoa.c:420`
- Declaration/interface: file-local prototype at `n_ftoa.c:39`
- Signature/API shape: `static int getnumsep(int digits)`
- Visibility: file-local
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Returns `(digits - ((digits % 3 == 0) ? 1 : 0)) / 3`, the number of comma group separators that would be inserted between integer digits.

## Important Boundaries

- Preconditions and assumptions: caller passes a non-negative integer digit count.
- Error/null/empty behavior: no error channel.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(1).

## Direct Call Sites

Exact source search found `fmtflt` as the production caller, only when `PRINT_F_QUOTE` grouping is active.

## Role In Callers

`fmtflt` uses this to include comma separators in padding width calculations. `JNtoA` does not set grouping flags, so this path is inactive for current JSON printing.

## Tests Observed

No focused grouping test was inspected; JSON number tests do not exercise grouping.

## Graph Links

- Graph node id: `function:n_ftoa.c:420:getnumsep`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
