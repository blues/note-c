# `cast`

## Status

Finalized. Source-inspected formatter numeric cast helper.

## Identity

- Kind: function
- Definition: `n_ftoa.c:462`
- Declaration/interface: file-local prototype at `n_ftoa.c:42`
- Signature/API shape: `static uintmax_t cast(JNUMBER value)`
- Visibility: file-local
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `UINTMAX_MAX`
- Data/type/global dependencies: none

## Behavior

Returns `UINTMAX_MAX` as an overflow sentinel when `value >= UINTMAX_MAX`. Otherwise casts `value` to `uintmax_t` and compensates for platforms that round floating-to-integer casts upward by returning `result - 1` when the cast result is greater than the original value.

## Important Boundaries

- Preconditions and assumptions: called with non-negative finite values in the normal formatter path.
- Error/null/empty behavior: overflow is represented by the `UINTMAX_MAX` sentinel.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(1).
- Portability/platform constraints: explicitly handles implementations with non-truncating floating-to-integer casts.

## Direct Call Sites

Exact source search found callers in `fmtflt` and `myround`.

## Role In Callers

`fmtflt` uses `cast` to split integer and fractional parts; `myround` uses it before half-up rounding.

## Tests Observed

Focused JSON number handling tests exercise this indirectly through `JPrintUnformatted`; direct `JNtoA` tests are weak because they do not assert formatter output.

## Graph Links

- Graph node id: `function:n_ftoa.c:462:cast`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
