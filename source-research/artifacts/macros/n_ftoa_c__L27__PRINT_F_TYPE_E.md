# `PRINT_F_TYPE_E`

## Status

Finalized. Source-inspected floating formatter exponent-style flag.

## Identity

- Kind: macro
- Definition: `n_ftoa.c:27`
- Declaration/interface: file-local preprocessor flag
- Public/exported names: none
- Macro aliases/wrappers: none found
- Signature/API shape: `#define PRINT_F_TYPE_E 0x0002`
- Visibility: private to `n_ftoa.c`
- Ownership/lifetime role: formatting option bit for `fmtflt`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: read from the `flags` bitmask in `fmtflt`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edge

## Behavior

When set, `fmtflt` selects exponent-style formatting by forcing `estyle`, computing an exponent, and emitting an exponent suffix.

## Important Boundaries

- Preconditions and assumptions: only meaningful in the internal formatter flag bitmask.
- Error/null/empty behavior: not a runtime function.
- Ownership and lifetime: no ownership.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit flag private to portable numeric formatting code.
- Performance shape: enables exponent computation and suffix emission.
- Security or parsing constraints: output formatting only.

## Direct Call Sites

Source search found the definition and `fmtflt` checks. No production caller currently sets this flag directly.

## Role In Callers

Dormant `%e`-style formatting option inside `fmtflt`; the public `JNtoA` path uses `%g`-style instead.

## Tests Observed

No direct `%e`/`PRINT_F_TYPE_E` focused test was found.

## Graph Links

- Graph node id: `macro:n_ftoa.c:27:PRINT_F_TYPE_E`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edge from `fmtflt`.

## Evidence Gaps

No unresolved source gaps for this node.
