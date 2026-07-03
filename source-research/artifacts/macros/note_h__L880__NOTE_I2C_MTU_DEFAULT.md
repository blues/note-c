# `NOTE_I2C_MTU_DEFAULT`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:880`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_I2C_MTU_DEFAULT`
- Macro aliases/wrappers: mirror `NOTE_I2C_MAX_DEFAULT` when not pre-defined.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Mirrors the selected `NOTE_I2C_MAX_DEFAULT` branch.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Provides the public MTU default consumed by `NoteI2CMax` when runtime `i2cMax` is zero.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Used by `NoteI2CMax` in `n_hooks.c:997`; I2C transport chunking sees it through `_I2CMax()`.

## Role In Callers

Connects compile-time default MTU policy to runtime I2C chunk sizes.

## Tests Observed

Focused `NoteI2CMax_test.cpp` and I2C chunk/reset tests cover default MTU behavior.

## Graph Links

- Graph node id: `macro:note.h:880:NOTE_I2C_MTU_DEFAULT`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `NoteI2CMax`.

## Evidence Gaps

No unresolved source gaps for this node.
