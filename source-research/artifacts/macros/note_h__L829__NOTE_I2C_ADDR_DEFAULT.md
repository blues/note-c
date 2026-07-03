# `NOTE_I2C_ADDR_DEFAULT`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:829`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_I2C_ADDR_DEFAULT`
- Macro aliases/wrappers: literal `0x17`.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, alias, attribute, or expression.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Provides the Notecard default I2C address. `NoteI2CAddress` returns it when runtime `i2cAddress` state is zero.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Used by `NoteI2CAddress` in `n_hooks.c:969`; I2C transport calls receive the effective address through `_I2CAddress` alias to `NoteI2CAddress`.

## Role In Callers

Defines the default runtime address without forcing callers to configure one.

## Tests Observed

Focused `NoteI2CAddress_test.cpp` covers default and explicit address behavior.

## Graph Links

- Graph node id: `macro:note.h:829:NOTE_I2C_ADDR_DEFAULT`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `NoteI2CAddress`.

## Evidence Gaps

No unresolved source gaps for this node.
