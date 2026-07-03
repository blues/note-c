# `_I2CMax`

## Status

Finalized. Source-inspected I2C MTU macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:248`
- Declaration/interface: internal macro alias in `n_lib.h`
- Signature/API shape: `_I2CMax`
- Visibility: internal macro
- Public/exported names: none
- Expands to: `NoteI2CMax`

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:994:NoteI2CMax` finalized
- External/system dependencies: none
- Data/type/global dependencies: inherits `NoteI2CMax` access to `i2cMax`
- Artifact coverage: macro definition, target function, direct callers, and tests inspected

## Behavior

Expands directly to `NoteI2CMax`, preserving all MTU defaulting and clamping behavior.

## Important Boundaries

- Preconditions and assumptions: none beyond target function state.
- Error/null/empty behavior: unset MTU resolves to default through target.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows `NoteI2CMax` global-state behavior.
- Portability/platform constraints: follows target compile-time MTU macros.
- Performance shape: O(1).
- Security or parsing constraints: none.

## Direct Call Sites

Exact production callers:

- `n_i2c.c:252` `_i2cNoteReset` receive chunk cap.
- `n_i2c.c:375` `_i2cChunkedReceive` initial read cap.
- `n_i2c.c:400` `_i2cChunkedReceive` follow-up read cap.
- `n_i2c.c:487` `_i2cChunkedTransmit` transmit chunk cap.

## Role In Callers

Keeps I2C transport code readable while routing MTU lookup through the public hook/helper function.

## Tests Observed

`NoteI2CMax_test.cpp` covers the target behavior, and `_i2cNoteReset_test.cpp` covers reset caller chunk caps.

## Graph Links

- Graph node id: `macro:n_lib.h:248:_I2CMax`
- Structured graph: `inventory/dependency-graph.json`
- Verified expands-to target: `NoteI2CMax`

## Evidence Gaps

No unresolved source gaps for this alias.
