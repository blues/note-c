# `PRINT_F_UP`

## Status

Finalized. Source-inspected floating formatter uppercase-output flag.

## Identity

- Kind: macro
- Definition: `n_ftoa.c:34`
- Declaration/interface: file-local preprocessor flag
- Public/exported names: none
- Macro aliases/wrappers: none found
- Signature/API shape: `#define PRINT_F_UP 0x0100`
- Visibility: private to `n_ftoa.c`
- Ownership/lifetime role: formatting option bit for `fmtflt`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: read from the `flags` bitmask in `fmtflt`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edge

## Behavior

When set, `fmtflt` emits uppercase `NAN`/`INF` and uppercase exponent marker `E`; when clear, it emits lowercase `nan`/`inf` and `e`.

## Important Boundaries

- Preconditions and assumptions: only meaningful in the internal formatter flag bitmask.
- Error/null/empty behavior: not a runtime function.
- Ownership and lifetime: no ownership.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit flag private to portable numeric formatting code.
- Performance shape: affects constant string and exponent character selection.
- Security or parsing constraints: output formatting only.

## Direct Call Sites

Source search found the definition and `fmtflt` checks. The current `JNtoA` path leaves this flag unset.

## Role In Callers

Dormant uppercase-formatting option inside `fmtflt`; note-c's public JSON number output uses lowercase exponent markers.

## Tests Observed

JSON number handling tests indirectly expect lowercase exponent output. No direct uppercase, NaN, or Inf `JNtoA` test was found; JSON printing converts NaN/Inf to `null` before `JNtoA`.

## Graph Links

- Graph node id: `macro:n_ftoa.c:34:PRINT_F_UP`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edge from `fmtflt`.

## Evidence Gaps

No unresolved source gaps for this node.
