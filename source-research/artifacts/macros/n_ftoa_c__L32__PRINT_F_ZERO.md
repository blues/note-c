# `PRINT_F_ZERO`

## Status

Finalized. Source-inspected floating formatter zero-padding flag.

## Identity

- Kind: macro
- Definition: `n_ftoa.c:32`
- Declaration/interface: file-local preprocessor flag
- Public/exported names: none
- Macro aliases/wrappers: none found
- Signature/API shape: `#define PRINT_F_ZERO 0x0040`
- Visibility: private to `n_ftoa.c`
- Ownership/lifetime role: formatting option bit for `fmtflt`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: read from the `flags` bitmask in `fmtflt`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edge

## Behavior

When set and left-justify is not set, `fmtflt` emits leading zero padding after any sign character.

## Important Boundaries

- Preconditions and assumptions: ignored when `PRINT_F_MINUS` left-justify handling is active.
- Error/null/empty behavior: not a runtime function.
- Ownership and lifetime: no ownership.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit flag private to portable numeric formatting code.
- Performance shape: affects padding branches only.
- Security or parsing constraints: output formatting only.

## Direct Call Sites

Source search found the definition and `fmtflt` check. No production caller currently sets this flag.

## Role In Callers

Dormant zero-padding formatting option inside `fmtflt`.

## Tests Observed

No focused padding test or production caller was found for this flag.

## Graph Links

- Graph node id: `macro:n_ftoa.c:32:PRINT_F_ZERO`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edge from `fmtflt`.

## Evidence Gaps

No unresolved source gaps for this node.
