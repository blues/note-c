# `NOTE_I2C_MTU_MAX`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:849`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_I2C_MTU_MAX`
- Macro aliases/wrappers: default mirror `NOTE_I2C_MAX_MAX` when not pre-defined.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Mirrors `NOTE_I2C_MAX_MAX` unless supplied externally.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Provides the public MTU maximum macro consumed by `NoteI2CMax`. If runtime `i2cMax` exceeds it, `NoteI2CMax` clamps and writes back the clamped value.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Used by `NoteI2CMax` in `n_hooks.c:1002`; I2C transport chunk sizes are capped via `_I2CMax()` alias calls.

## Role In Callers

Connects compile-time maximum configuration to runtime I2C receive/transmit chunk limits.

## Tests Observed

Focused `NoteI2CMax_test.cpp`, `_i2cChunkedReceive_test.cpp`, `_i2cChunkedTransmit_test.cpp`, and `_i2cNoteReset_test.cpp` cover effective limits.

## Graph Links

- Graph node id: `macro:note.h:849:NOTE_I2C_MTU_MAX`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `NoteI2CMax`.

## Evidence Gaps

No unresolved source gaps for this node.
