# `F4`

## Status

Finalized. Source-inspected MD5 round macro.

## Identity

- Kind: macro
- Definition: `n_md5.c:190`
- Signature/API shape: `#define F4(x, y, z) (y ^ (x | ~z))`
- Visibility: local to `n_md5.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: primitive bitwise OR, XOR, and complement
- Data/type/global dependencies: none

## Behavior

Computes the MD5 I/F4 round expression using `y ^ (x | ~z)`.

## Important Boundaries

- Preconditions and assumptions: function-like macro; arguments are integer expressions.
- Error/null/empty behavior: not applicable.
- Ownership and lifetime: no memory ownership.
- Concurrency/global state: no state.
- Performance shape: inline macro expression.

## Direct Call Sites

Exact source search `rg -n "\bF4\b" n_md5.c` found the definition at `n_md5.c:190` and fourth-round `MD5STEP(F4, ...)` uses at `n_md5.c:267`-`n_md5.c:282`.

## Role In Callers

`NoteMD5Transform` passes `F4` as the function-like macro argument for the fourth 16 `MD5STEP` invocations.

## Tests Observed

None. Macro behavior is covered only through higher-level MD5 callers in this research slice.

## Graph Links

- Graph node id: `macro:n_md5.c:190:F4`
- Structured graph: `inventory/dependency-graph.json`
- Verified macro-use edges from `NoteMD5Transform` and `MD5STEP` were repaired from source inspection.

## Evidence Gaps

No unresolved source gaps for this node.
