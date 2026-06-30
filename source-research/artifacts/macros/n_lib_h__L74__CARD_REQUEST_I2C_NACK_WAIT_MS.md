# `CARD_REQUEST_I2C_NACK_WAIT_MS`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:74`
- Declaration/interface: I2C NACK wait macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `1000` milliseconds.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: I2C reset path.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Controls how long `_i2cNoteReset` waits after an I2C transmit NACK before retrying sync.

## Important Boundaries

- Compile-time constant.
- Only used on the I2C reset NACK path.
- Does not affect normal transmit segmentation.

## Direct Call Sites

Used by `_i2cNoteReset` after `_I2CTransmit` returns an error while sending the reset newline.

## Role In Callers

Relieves bus/device stress before retrying a Notecard sync attempt.

## Tests Observed

Focused I2C reset tests cover reset retry behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:74:CARD_REQUEST_I2C_NACK_WAIT_MS`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `_i2cNoteReset`.

## Evidence Gaps

No unresolved source gaps for this node.
