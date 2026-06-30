# `_i2cNoteChunkedTransmit`

## Status

Finalized. Source-inspected I2C chunked transmit dispatch wrapper, lock macros, dispatch role, and focused tests.

## Identity

- Kind: function
- Definition: `n_i2c.c:456`
- Declaration/interface: `n_lib.h:131`
- Public/exported names: none; internal adapter target selected by `_noteSetActiveInterface`
- Macro aliases/wrappers: selected as `notecardChunkedTransmit` for `NOTE_C_INTERFACE_I2C`
- Signature/API shape: `const char *_i2cNoteChunkedTransmit(const uint8_t *buffer, uint32_t size, bool delay)`
- Visibility: internal SDK transport function declared in `n_lib.h`
- Ownership/lifetime role: lock-protected forwarding wrapper; caller owns transmit buffer

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:245:_LockI2C` finalized
- `function:n_i2c.c:475:_i2cChunkedTransmit` finalized in this slice
- `macro:n_lib.h:246:_UnlockI2C` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: I2C lock/unlock hook state through `_LockI2C` and `_UnlockI2C`
- Artifact coverage: source, declaration, dispatch assignment, callee artifacts, and focused tests inspected

## Behavior

Acquires the I2C lock with `_LockI2C()`, forwards `buffer`, `size`, and `delay` unchanged to `_i2cChunkedTransmit`, releases the lock with `_UnlockI2C()`, and returns the callee error string unchanged.

## Important Boundaries

- Preconditions and assumptions: inherits `_i2cChunkedTransmit` buffer requirements; no local validation is performed.
- Error/null/empty behavior: all success/error behavior comes from `_i2cChunkedTransmit`.
- Ownership and lifetime: no allocation or ownership transfer; caller owns the buffer.
- Concurrency/global state: provides the I2C critical section around the private transmit loop. Unlock runs after the callee returns.
- Portability/platform constraints: lock/unlock and transmit behavior remain behind hooks and adapter internals.
- Performance shape: O(1) wrapper plus `_i2cChunkedTransmit` loop cost.
- Security or parsing constraints: no parsing; inherits chunk sizing and error-reset behavior from `_i2cChunkedTransmit`.

## Direct Call Sites

Exact source search found production dispatch assignment:

- `n_hooks.c:229` `_noteSetActiveInterface` assigns `notecardChunkedTransmit = _i2cNoteChunkedTransmit` for `NOTE_C_INTERFACE_I2C`.

The selected function pointer is called by `n_hooks.c:1092` `_noteChunkedTransmit`, which is exposed internally through the `_ChunkedTransmit` macro.

## Role In Callers

This is the lock-protected I2C adapter endpoint for internal chunked transmit dispatch. Higher-level binary transmit code reaches it indirectly when I2C is the active interface.

## Tests Observed

Focused `test/src/_i2cNoteChunkedTransmit_test.cpp` fakes `_i2cChunkedTransmit`, `NoteLockI2C`, and `NoteUnlockI2C`; it verifies exact argument forwarding, unchanged return propagation, and balanced lock/unlock calls. `_noteSetActiveInterface_test.cpp` verifies I2C dispatch pointer installation.

## Graph Links

- Graph node id: `function:n_i2c.c:456:_i2cNoteChunkedTransmit`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockI2C`, `_i2cChunkedTransmit`, and `_UnlockI2C`.

## Evidence Gaps

No unresolved source gaps for this node. `_noteChunkedTransmit` and `_ChunkedTransmit` remain separate preliminary dispatcher artifacts.
