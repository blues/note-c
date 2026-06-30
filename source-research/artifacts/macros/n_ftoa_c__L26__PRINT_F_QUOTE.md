# `PRINT_F_QUOTE`

## Status

Finalized. Source-inspected floating formatter thousands-separator flag.

## Identity

- Kind: macro
- Definition: `n_ftoa.c:26`
- Declaration/interface: file-local preprocessor flag
- Public/exported names: none
- Macro aliases/wrappers: none found
- Signature/API shape: `#define PRINT_F_QUOTE 0x0001`
- Visibility: private to `n_ftoa.c`
- Ownership/lifetime role: formatting option bit for `fmtflt`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: read from the `flags` bitmask in `fmtflt`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edge

## Behavior

When set, `fmtflt` enables thousands separators by computing separator count and emitting commas through `printsep` while writing the integer part.

## Important Boundaries

- Preconditions and assumptions: only meaningful in the internal formatter flag bitmask.
- Error/null/empty behavior: not a runtime function.
- Ownership and lifetime: no ownership.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit flag private to portable numeric formatting code.
- Performance shape: adds separator counting and comma emission.
- Security or parsing constraints: no parsing; output formatting only.

## Direct Call Sites

Source search found the definition and `fmtflt` checks. No production caller currently sets this flag.

## Role In Callers

Dormant formatting option for grouped integer output inside `fmtflt`.

## Tests Observed

No focused test or production caller was found for thousands-separator behavior.

## Graph Links

- Graph node id: `macro:n_ftoa.c:26:PRINT_F_QUOTE`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edge from `fmtflt`.

## Evidence Gaps

No unresolved source gaps for this node.
