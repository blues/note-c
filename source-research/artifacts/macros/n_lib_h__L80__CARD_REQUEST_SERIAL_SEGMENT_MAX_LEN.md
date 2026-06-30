# `CARD_REQUEST_SERIAL_SEGMENT_MAX_LEN`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:80`
- Declaration/interface: Serial request segmentation macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `250` bytes.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: serial translation unit.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Caps serial transmit segments, with a compile-time guard that rejects impossible platforms where the cap exceeds `SIZE_MAX`.

## Important Boundaries

- Compile-time constant.
- Applies only to serial transmit segmentation.
- `_serialChunkedTransmit` casts the capped segment length to `size_t`.

## Direct Call Sites

Used by `_serialChunkedTransmit` in a preprocessor size guard and in the segment length selection branch.

## Role In Callers

Prevents oversized serial writes and keeps segment sizing portable.

## Tests Observed

Focused serial transaction tests cover segmented transmit behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:80:CARD_REQUEST_SERIAL_SEGMENT_MAX_LEN`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `_serialChunkedTransmit`.

## Evidence Gaps

No unresolved source gaps for this node.
