# `CARD_RESET_DRAIN_MS`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:92`
- Declaration/interface: reset drain duration macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `500` milliseconds.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: I2C and serial reset paths.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Controls how long reset logic drains incoming bytes while deciding whether the Notecard is synchronized.

## Important Boundaries

- Compile-time constant.
- Used by both I2C and serial reset loops.
- Serial reset also waits this duration before resetting again after bad/no data.

## Direct Call Sites

Used by `_i2cNoteReset` and `_serialNoteReset` drain loops.

## Role In Callers

Defines the observation window for reset synchronization.

## Tests Observed

Focused I2C and serial reset tests exercise reset drain behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:92:CARD_RESET_DRAIN_MS`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to `_i2cNoteReset` and `_serialNoteReset`.

## Evidence Gaps

No unresolved source gaps for this node.
