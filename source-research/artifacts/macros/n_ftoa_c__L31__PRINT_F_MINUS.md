# `PRINT_F_MINUS`

## Status

Finalized. Source-inspected formatter left-justify flag.

## Identity

- Kind: macro
- Definition: `n_ftoa.c:31`
- Declaration/interface: file-local preprocessor flag
- Public/exported names: none
- Macro aliases/wrappers: none found
- Signature/API shape: `#define PRINT_F_MINUS 0x0020`
- Visibility: private to `n_ftoa.c`
- Ownership/lifetime role: formatting option bit for numeric and string formatting helpers

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: read from the `flags` bitmask in `fmtflt` and `fmtstr`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

When set, `fmtflt` and `fmtstr` left-justify output by making padding negative and emitting trailing spaces after content.

## Important Boundaries

- Preconditions and assumptions: only meaningful in the internal formatter flag bitmask.
- Error/null/empty behavior: not a runtime function.
- Ownership and lifetime: no ownership.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit flag private to portable formatting code.
- Performance shape: affects padding branches only.
- Security or parsing constraints: output formatting only.

## Direct Call Sites

Source search found the definition and checks in `fmtflt` and `fmtstr`. No production caller currently sets this flag.

## Role In Callers

Dormant left-justify formatting option for both numeric and string helper paths.

## Tests Observed

No focused padding test or production caller was found for this flag.

## Graph Links

- Graph node id: `macro:n_ftoa.c:31:PRINT_F_MINUS`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `fmtflt` and `fmtstr`.

## Evidence Gaps

No unresolved source gaps for this node.
