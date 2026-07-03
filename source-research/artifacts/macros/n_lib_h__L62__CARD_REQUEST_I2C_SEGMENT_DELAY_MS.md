# `CARD_REQUEST_I2C_SEGMENT_DELAY_MS`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:62`
- Declaration/interface: I2C segment pacing macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `250` milliseconds.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: I2C translation unit.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Adds pacing around I2C reset and between I2C request segments when delay mode is active.

## Important Boundaries

- Compile-time constant.
- Used for reset pacing and transmit back-pressure.
- Skipped in transmit when the segment loop is complete or delay is disabled.

## Direct Call Sites

Used by `_i2cNoteReset` before reset/receive retry paths and by `_i2cChunkedTransmit` between I2C segments.

## Role In Callers

Protects the Notecard from host-side I2C traffic arriving faster than it can process.

## Tests Observed

Focused I2C reset/chunking tests exercise delayed transmit paths.

## Graph Links

- Graph node id: `macro:n_lib.h:62:CARD_REQUEST_I2C_SEGMENT_DELAY_MS`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to `_i2cNoteReset` and `_i2cChunkedTransmit`.

## Evidence Gaps

No unresolved source gaps for this node.
