# `NOTE_I2C_HEADER_SIZE`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:833`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_I2C_HEADER_SIZE`
- Macro aliases/wrappers: default literal `2` unless pre-defined.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, alias, attribute, or expression.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the serial-to-I2C protocol header size used to compute the generic maximum MTU as `UCHAR_MAX - NOTE_I2C_HEADER_SIZE`.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Referenced by the default `NOTE_I2C_MAX_MAX` branch at `note.h:845`.

## Role In Callers

Keeps the protocol header subtraction configurable for compile-time platform ports.

## Tests Observed

No focused test directly overrides this macro; I2C max tests cover default effective limits.

## Graph Links

- Graph node id: `macro:note.h:833:NOTE_I2C_HEADER_SIZE`
- Structured graph: `inventory/dependency-graph.json`
- Verified macro dependency edge to default `NOTE_I2C_MAX_MAX`.

## Evidence Gaps

No unresolved source gaps for this node.
