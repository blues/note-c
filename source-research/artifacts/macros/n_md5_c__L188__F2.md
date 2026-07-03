# `F2`

## Status

Finalized. Source-inspected MD5 round macro.

## Identity

- Kind: macro
- Definition: `n_md5.c:188`
- Signature/API shape: `#define F2(x, y, z) F1(z, x, y)`
- Visibility: local to `n_md5.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: `macro:n_md5.c:187:F1` by expansion
- External/system dependencies: none beyond the expanded `F1` expression
- Data/type/global dependencies: none

## Behavior

Defines the MD5 G/F2 round function by reusing `F1` with rotated arguments: `F1(z, x, y)`.

## Important Boundaries

- Preconditions and assumptions: function-like macro; argument evaluation follows macro expansion.
- Error/null/empty behavior: not applicable.
- Ownership and lifetime: no memory ownership.
- Concurrency/global state: no state.
- Performance shape: inline macro expression.

## Direct Call Sites

Exact source search `rg -n "\bF2\b" n_md5.c` found the definition at `n_md5.c:188` and second-round `MD5STEP(F2, ...)` uses at `n_md5.c:233`-`n_md5.c:248`.

## Role In Callers

`NoteMD5Transform` passes `F2` as the function-like macro argument for the second 16 `MD5STEP` invocations.

## Tests Observed

None. Macro behavior is covered only through higher-level MD5 callers in this research slice.

## Graph Links

- Graph node id: `macro:n_md5.c:188:F2`
- Structured graph: `inventory/dependency-graph.json`
- Verified macro-use edges from `NoteMD5Transform` and `MD5STEP` were repaired from source inspection.

## Evidence Gaps

No unresolved source gaps for this node.
