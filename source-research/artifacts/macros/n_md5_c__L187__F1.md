# `F1`

## Status

Finalized. Source-inspected MD5 round macro.

## Identity

- Kind: macro
- Definition: `n_md5.c:187`
- Signature/API shape: `#define F1(x, y, z) (z ^ (x & (y ^ z)))`
- Visibility: local to `n_md5.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: primitive bitwise operations
- Data/type/global dependencies: none

## Behavior

Computes the optimized MD5 F function for one round expression using bitwise XOR, AND, and complement-equivalent algebra through `z ^ (x & (y ^ z))`.

## Important Boundaries

- Preconditions and assumptions: macro arguments may be evaluated more than once by expansion contexts; callers pass local integer variables.
- Error/null/empty behavior: not applicable.
- Ownership and lifetime: no memory ownership.
- Concurrency/global state: no state.
- Performance shape: inline macro expression.

## Direct Call Sites

Exact source search `rg -n "\bF1\b" n_md5.c` found the definition at `n_md5.c:187`, `F2` delegating to `F1` at `n_md5.c:188`, and first-round `MD5STEP(F1, ...)` uses at `n_md5.c:216`-`n_md5.c:231`.

## Role In Callers

`NoteMD5Transform` passes `F1` as the function-like macro argument for the first 16 `MD5STEP` invocations. `F2` also expands through `F1(z, x, y)`.

## Tests Observed

None. Macro behavior is covered only through higher-level MD5 callers in this research slice.

## Graph Links

- Graph node id: `macro:n_md5.c:187:F1`
- Structured graph: `inventory/dependency-graph.json`
- Verified macro-use edges from `NoteMD5Transform` and `MD5STEP` were repaired from source inspection.

## Evidence Gaps

No unresolved source gaps for this node.
