# `PRINT_F_PLUS`

## Status

Finalized. Source-inspected floating formatter explicit-plus flag.

## Identity

- Kind: macro
- Definition: `n_ftoa.c:30`
- Declaration/interface: file-local preprocessor flag
- Public/exported names: none
- Macro aliases/wrappers: none found
- Signature/API shape: `#define PRINT_F_PLUS 0x0010`
- Visibility: private to `n_ftoa.c`
- Ownership/lifetime role: formatting option bit for `fmtflt`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: read from the `flags` bitmask in `fmtflt`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edge

## Behavior

When set and the value is nonnegative, `fmtflt` emits a leading `+` sign.

## Important Boundaries

- Preconditions and assumptions: only meaningful in the internal formatter flag bitmask.
- Error/null/empty behavior: not a runtime function.
- Ownership and lifetime: no ownership.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit flag private to portable numeric formatting code.
- Performance shape: one sign-branch effect.
- Security or parsing constraints: output formatting only.

## Direct Call Sites

Source search found the definition and `fmtflt` check. No production caller currently sets this flag.

## Role In Callers

Dormant sign-formatting option inside `fmtflt`.

## Tests Observed

No focused test or production caller was found for explicit-plus output.

## Graph Links

- Graph node id: `macro:n_ftoa.c:30:PRINT_F_PLUS`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edge from `fmtflt`.

## Evidence Gaps

No unresolved source gaps for this node.
