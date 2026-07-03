# `_i2cNoteChunkedReceive`

## Status

Finalized. Source-inspected I2C chunked receive dispatch wrapper, lock macros, direct dispatch role, and focused tests.

## Identity

- Kind: function
- Definition: `n_i2c.c:338`
- Declaration/interface: `n_lib.h:130`
- Public/exported names: none; internal adapter target selected by `_noteSetActiveInterface`
- Macro aliases/wrappers: selected as `notecardChunkedReceive` for `NOTE_C_INTERFACE_I2C`
- Signature/API shape: `const char *_i2cNoteChunkedReceive(uint8_t *buffer, uint32_t *size, bool delay, uint32_t timeoutMs, uint32_t *available)`
- Visibility: internal SDK transport function declared in `n_lib.h`
- Ownership/lifetime role: lock-protected forwarding wrapper; caller owns buffer and in/out counters

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:245:_LockI2C` finalized
- `function:n_i2c.c:365:_i2cChunkedReceive` finalized in this slice
- `macro:n_lib.h:246:_UnlockI2C` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: I2C lock/unlock hook state through `_LockI2C` and `_UnlockI2C`
- Artifact coverage: source, declaration, dispatch assignment, callee artifacts, and focused tests inspected

## Behavior

Acquires the I2C lock with `_LockI2C()`, forwards `buffer`, `size`, `delay`, `timeoutMs`, and `available` unchanged to `_i2cChunkedReceive`, releases the lock with `_UnlockI2C()`, and returns the callee error string unchanged.

## Important Boundaries

- Preconditions and assumptions: inherits `_i2cChunkedReceive` pointer and buffer requirements; no local validation is performed.
- Error/null/empty behavior: all success/error behavior, `*size`, and `*available` mutations come from `_i2cChunkedReceive`.
- Ownership and lifetime: no allocation or ownership transfer; caller owns buffer and counters.
- Concurrency/global state: provides the I2C critical section around the private receive loop. Unlock runs after the callee returns.
- Portability/platform constraints: lock/unlock and receive behavior remain behind hooks and adapter internals.
- Performance shape: O(1) wrapper plus `_i2cChunkedReceive` receive loop cost.
- Security or parsing constraints: no parsing; inherits newline and buffer-boundary behavior from `_i2cChunkedReceive`.

## Direct Call Sites

Exact source search found production dispatch assignment:

- `n_hooks.c:228` `_noteSetActiveInterface` assigns `notecardChunkedReceive = _i2cNoteChunkedReceive` for `NOTE_C_INTERFACE_I2C`.

The selected function pointer is called by `n_hooks.c:1074` `_noteChunkedReceive`, which is exposed internally through the finalized `_ChunkedReceive` macro.

## Role In Callers

This is the lock-protected I2C adapter endpoint for internal chunked receive dispatch. It lets higher-level binary receive code use `_ChunkedReceive` without knowing whether the active transport is I2C or serial.

## Tests Observed

Focused `test/src/_i2cNoteChunkedReceive_test.cpp` fakes `_i2cChunkedReceive`, `NoteLockI2C`, and `NoteUnlockI2C`; it verifies exact argument forwarding, unchanged return/result propagation, and balanced lock/unlock calls.

## Graph Links

- Graph node id: `function:n_i2c.c:338:_i2cNoteChunkedReceive`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockI2C`, `_i2cChunkedReceive`, and `_UnlockI2C`.

## Evidence Gaps

No unresolved source gaps for this node.
