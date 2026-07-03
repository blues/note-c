# `PRINT_F_TYPE_G`

## Status

Finalized. Source-inspected floating formatter significant-digits flag.

## Identity

- Kind: macro
- Definition: `n_ftoa.c:28`
- Declaration/interface: file-local preprocessor flag
- Public/exported names: none
- Macro aliases/wrappers: none found
- Signature/API shape: `#define PRINT_F_TYPE_G 0x0004`
- Visibility: private to `n_ftoa.c`
- Ownership/lifetime role: active formatting option bit used by `JNtoA`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: set in `JNtoA` and read by `fmtflt`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

`JNtoA` sets this flag before calling `fmtflt`. In `fmtflt`, it enables `%g`-like significant-digit behavior: precision counts significant digits, exponent-vs-fixed style is selected based on exponent and precision, and trailing fractional zeros are omitted unless `PRINT_F_NUM` is also set.

## Important Boundaries

- Preconditions and assumptions: public numeric printing path depends on this flag for JSON-compatible compact number text.
- Error/null/empty behavior: not a runtime function.
- Ownership and lifetime: no ownership.
- Concurrency/global state: none.
- Portability/platform constraints: integer bit flag private to portable numeric formatting code.
- Performance shape: can require a recalculation pass when `%g` chooses fixed style after exponent analysis.
- Security or parsing constraints: output formatting only.

## Direct Call Sites

Source search found `JNtoA` sets this flag and `fmtflt` reads it. No other production setter was found.

## Role In Callers

This is the active flag behind note-c's public `JNtoA` number-to-string conversion.

## Tests Observed

JSON number handling tests indirectly verify lowercase exponent output and compact number printing. `JNtoA_test` calls `JNtoA`; source inspection noted a test assertion pattern that appears to parse the expected string rather than the produced buffer in at least one path.

## Graph Links

- Graph node id: `macro:n_ftoa.c:28:PRINT_F_TYPE_G`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JNtoA` and `fmtflt`.

## Evidence Gaps

No unresolved source gaps for this node.
