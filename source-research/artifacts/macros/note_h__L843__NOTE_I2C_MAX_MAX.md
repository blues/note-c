# `NOTE_I2C_MAX_MAX`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:843`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_I2C_MAX_MAX`
- Macro aliases/wrappers: compatibility branch `NOTE_I2C_MTU_MAX` when `NOTE_I2C_MTU_MAX` is pre-defined and `NOTE_I2C_MAX_MAX` is not.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Compile branch depends on a pre-defined `NOTE_I2C_MTU_MAX`.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Mirrors the older/newer `NOTE_I2C_MTU_MAX` compile-time setting into `NOTE_I2C_MAX_MAX` for compatibility. This branch is mutually exclusive with the default `UCHAR_MAX - header` branch.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

No direct runtime source reference to this branch; it feeds the compatibility macro lattice used by `NOTE_I2C_MTU_MAX` and `NoteI2CMax`.

## Role In Callers

Maintains source compatibility for code defining either max macro name before including `note.h`.

## Tests Observed

Focused I2C max tests exercise effective MTU behavior, not compile-time branch injection.

## Graph Links

- Graph node id: `macro:note.h:843:NOTE_I2C_MAX_MAX`
- Structured graph: `inventory/dependency-graph.json`
- Recorded compatibility alias edge in the graph.

## Evidence Gaps

No unresolved source gaps for this node.
