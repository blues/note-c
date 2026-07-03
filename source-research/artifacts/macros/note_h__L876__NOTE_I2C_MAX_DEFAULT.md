# `NOTE_I2C_MAX_DEFAULT`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:876`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_I2C_MAX_DEFAULT`
- Macro aliases/wrappers: default literal `30`
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, alias, attribute, or expression.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Uses a conservative default chunk size for broad Arduino compatibility with 32-byte I2C buffers and the 2-byte protocol header.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

No direct runtime reference to this branch name; `NOTE_I2C_MTU_DEFAULT` mirrors the selected branch and `NoteI2CMax` consumes that public MTU default.

## Role In Callers

Defines the compile-time default chunk size branch before runtime state can override it.

## Tests Observed

Focused I2C max/chunk tests cover default effective MTU behavior.

## Graph Links

- Graph node id: `macro:note.h:876:NOTE_I2C_MAX_DEFAULT`
- Structured graph: `inventory/dependency-graph.json`
- Recorded branch/default macro dependency edges in the graph.

## Evidence Gaps

No unresolved source gaps for this node.
