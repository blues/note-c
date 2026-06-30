# `NoteI2CMax`

## Status

Finalized. Source-inspected public I2C MTU accessor.

## Identity

- Kind: function
- Definition: `n_hooks.c:994`
- Declaration/interface: `note.h:1105`
- Signature/API shape: `uint32_t NoteI2CMax(void)`
- Visibility: public SDK hook/helper API
- Public/exported names: `NoteI2CMax`
- Macro aliases/wrappers: `_I2CMax` in `n_lib.h:248`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads and may write file-local `i2cMax` (`n_hooks.c:152`); uses `NOTE_I2C_MTU_DEFAULT` and `NOTE_I2C_MTU_MAX`
- Artifact coverage: source, public declaration, macro alias, state writers/readers, direct callers, and focused tests inspected

## Behavior

Returns `NOTE_I2C_MTU_DEFAULT` when `i2cMax == 0`. If `i2cMax > NOTE_I2C_MTU_MAX`, mutates `i2cMax` down to `NOTE_I2C_MTU_MAX` and returns the clamped value. Otherwise returns the stored `i2cMax`.

## Important Boundaries

- Preconditions and assumptions: `i2cMax` is maintained by I2C hook setup and MTU setter APIs.
- Error/null/empty behavior: unset or explicitly zero MTU resolves to the default.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads and may mutate `i2cMax` without taking the note lock.
- Portability/platform constraints: default/max values are compile-time-configurable I2C MTU macros in `note.h`.
- Performance shape: O(1).
- Security or parsing constraints: none.

## Direct Call Sites

Exact production source calls through the `_I2CMax` macro alias in `n_i2c.c`:

- `n_i2c.c:252` caps reset receive chunk length.
- `n_i2c.c:375` caps chunked receive request length.
- `n_i2c.c:400` caps follow-up chunked receive request length.
- `n_i2c.c:487` caps chunked transmit length.

State writers/readers include `NoteSetFnI2C`, `NoteSetFnI2CDefault`, `NoteSetI2CMtu`, `NoteGetFnI2C`, and `NoteGetI2CMtu`.

## Role In Callers

Defines the transport segment size limit used by I2C reset, receive, and transmit paths.

## Tests Observed

- `test/src/NoteI2CMax_test.cpp:23-58` covers default return, clamp-to-max, and returning an explicit value.
- `test/src/_i2cNoteReset_test.cpp:381-409` checks reset receive sizing is capped by `NoteI2CMax()`.

## Graph Links

- Graph node id: `function:n_hooks.c:994:NoteI2CMax`
- Structured graph: `inventory/dependency-graph.json`
- Verified alias: `_I2CMax`

## Evidence Gaps

No unresolved source gaps for this node. Thread-safety of unlocked `i2cMax` mutation is not covered by focused tests.
