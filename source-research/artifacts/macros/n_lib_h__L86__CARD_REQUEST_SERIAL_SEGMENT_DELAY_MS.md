# `CARD_REQUEST_SERIAL_SEGMENT_DELAY_MS`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:86`
- Declaration/interface: Serial segment pacing macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `250` milliseconds.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: serial translation unit.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Adds pacing before serial reset and between serial request segments when delay mode is active.

## Important Boundaries

- Compile-time constant.
- Skipped at the end of the final transmit segment.
- Separate from reset drain duration.

## Direct Call Sites

Used by `_serialNoteReset` before reset and by `_serialChunkedTransmit` between serial segments.

## Role In Callers

Protects the Notecard serial input path from host writes arriving too quickly.

## Tests Observed

Focused serial reset/transaction tests exercise delayed transmit paths.

## Graph Links

- Graph node id: `macro:n_lib.h:86:CARD_REQUEST_SERIAL_SEGMENT_DELAY_MS`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to `_serialNoteReset` and `_serialChunkedTransmit`.

## Evidence Gaps

No unresolved source gaps for this node.
