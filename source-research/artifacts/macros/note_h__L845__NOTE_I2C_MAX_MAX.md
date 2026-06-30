# `NOTE_I2C_MAX_MAX`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:845`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_I2C_MAX_MAX`
- Macro aliases/wrappers: default expression `(UCHAR_MAX - NOTE_I2C_HEADER_SIZE)`, normally `253`.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Depends on `UCHAR_MAX` and `NOTE_I2C_HEADER_SIZE`.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Computes the generic maximum Notecard I2C payload size from the unsigned-char protocol byte range minus the serial-to-I2C header.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Referenced by `NOTE_I2C_MTU_MAX` default mirror and ultimately by `NoteI2CMax` clamp behavior.

## Role In Callers

Defines the portable upper bound for I2C chunking when no platform-specific maximum is supplied.

## Tests Observed

Focused `NoteI2CMax_test.cpp` and I2C chunk/reset tests cover the effective max behavior.

## Graph Links

- Graph node id: `macro:note.h:845:NOTE_I2C_MAX_MAX`
- Structured graph: `inventory/dependency-graph.json`
- Verified dependency edge to `NOTE_I2C_HEADER_SIZE`.

## Evidence Gaps

No unresolved source gaps for this node.
