# `CARD_RESET_SYNC_RETRIES`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:104`
- Declaration/interface: reset sync retry macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `10` sync attempts.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: I2C and serial reset paths.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Caps how many reset-sync newline/drain attempts the I/O reset paths make before giving up.

## Important Boundaries

- Compile-time constant.
- Applied independently in I2C and serial reset loops.
- Successful sync breaks before the cap.

## Direct Call Sites

Used by `_i2cNoteReset` and `_serialNoteReset` retry loops.

## Role In Callers

Bounds reset recovery work so a missing or unsynchronized Notecard does not loop forever.

## Tests Observed

Focused I2C and serial reset tests exercise retry limits.

## Graph Links

- Graph node id: `macro:n_lib.h:104:CARD_RESET_SYNC_RETRIES`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to `_i2cNoteReset` and `_serialNoteReset`.

## Evidence Gaps

No unresolved source gaps for this node.
