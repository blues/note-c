# `F3`

## Status

Finalized. Source-inspected MD5 round macro.

## Identity

- Kind: macro
- Definition: `n_md5.c:189`
- Signature/API shape: `#define F3(x, y, z) (x ^ y ^ z)`
- Visibility: local to `n_md5.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: primitive bitwise XOR
- Data/type/global dependencies: none

## Behavior

Computes the MD5 H/F3 round expression by XOR-ing the three input words.

## Important Boundaries

- Preconditions and assumptions: function-like macro; arguments are integer expressions.
- Error/null/empty behavior: not applicable.
- Ownership and lifetime: no memory ownership.
- Concurrency/global state: no state.
- Performance shape: inline macro expression.

## Direct Call Sites

Exact source search `rg -n "\bF3\b" n_md5.c` found the definition at `n_md5.c:189` and third-round `MD5STEP(F3, ...)` uses at `n_md5.c:250`-`n_md5.c:265`.

## Role In Callers

`NoteMD5Transform` passes `F3` as the function-like macro argument for the third 16 `MD5STEP` invocations.

## Tests Observed

None. Macro behavior is covered only through higher-level MD5 callers in this research slice.

## Graph Links

- Graph node id: `macro:n_md5.c:189:F3`
- Structured graph: `inventory/dependency-graph.json`
- Verified macro-use edges from `NoteMD5Transform` and `MD5STEP` were repaired from source inspection.

## Evidence Gaps

No unresolved source gaps for this node.
