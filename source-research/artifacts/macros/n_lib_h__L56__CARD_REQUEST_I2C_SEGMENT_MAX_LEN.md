# `CARD_REQUEST_I2C_SEGMENT_MAX_LEN`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:56`
- Declaration/interface: I2C request segmentation macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `250` bytes.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: I2C translation unit.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Caps each I2C transmit segment so large requests are split before being sent to the Notecard.

## Important Boundaries

- Compile-time constant.
- Applied only to I2C transmit segmentation.
- Final segment may be shorter than the cap.

## Direct Call Sites

Used in `_i2cChunkedTransmit` to reduce `sentInSegment` when the remaining chunk exceeds the cap.

## Role In Callers

Prevents overwhelming the Notecard I2C interrupt/request buffer.

## Tests Observed

Focused I2C chunked transmit tests cover segmentation behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:56:CARD_REQUEST_I2C_SEGMENT_MAX_LEN`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `_i2cChunkedTransmit`.

## Evidence Gaps

No unresolved source gaps for this node.
