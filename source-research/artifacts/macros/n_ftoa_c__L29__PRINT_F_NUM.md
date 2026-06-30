# `PRINT_F_NUM`

## Status

Finalized. Source-inspected floating formatter force-decimal flag.

## Identity

- Kind: macro
- Definition: `n_ftoa.c:29`
- Declaration/interface: file-local preprocessor flag
- Public/exported names: none
- Macro aliases/wrappers: none found
- Signature/API shape: `#define PRINT_F_NUM 0x0008`
- Visibility: private to `n_ftoa.c`
- Ownership/lifetime role: formatting option bit for `fmtflt`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: read from the `flags` bitmask in `fmtflt`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edge

## Behavior

When set, `fmtflt` prevents `%g` trailing-zero omission and forces decimal-point emission even when fractional precision would otherwise be omitted.

## Important Boundaries

- Preconditions and assumptions: only meaningful in the internal formatter flag bitmask.
- Error/null/empty behavior: not a runtime function.
- Ownership and lifetime: no ownership.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit flag private to portable numeric formatting code.
- Performance shape: affects formatting branches only.
- Security or parsing constraints: output formatting only.

## Direct Call Sites

Source search found the definition and `fmtflt` checks. No production caller currently sets this flag.

## Role In Callers

Dormant `#`-style formatting option inside `fmtflt`.

## Tests Observed

No focused test or production caller was found for forced-decimal behavior.

## Graph Links

- Graph node id: `macro:n_ftoa.c:29:PRINT_F_NUM`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edge from `fmtflt`.

## Evidence Gaps

No unresolved source gaps for this node.
