# `_i2cChunkedReceive`

## Status

Finalized. Source-inspected private I2C chunked receive loop, direct callers, dependency macros, and focused tests.

## Identity

- Kind: function
- Definition: `n_i2c.c:365`
- Declaration/interface: `n_lib.h:128`
- Public/exported names: none; internal transport helper
- Macro aliases/wrappers: wrapped by `_i2cNoteChunkedReceive` for lock-protected I2C dispatch
- Signature/API shape: `const char *_i2cChunkedReceive(uint8_t *buffer, uint32_t *size, bool delay, uint32_t timeoutMs, uint32_t *available)`
- Visibility: internal SDK transport function declared in `n_lib.h`
- Ownership/lifetime role: writes into caller-owned buffer and mutates caller-owned `size`/`available` counters

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:243:_GetMs` finalized
- `macro:n_lib.h:248:_I2CMax` finalized
- `macro:n_lib.h:236:_I2CReceive` finalized
- `macro:n_lib.h:247:_I2CAddress` finalized
- `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
- `macro:note.h:998:NOTE_C_LOG_WARN` finalized
- `macro:note.h:75:ERRSTR` finalized
- `macro:note.h:78:ERRSTR` finalized
- `macro:n_lib.h:244:_DelayMs` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: I2C hook state through `_I2CReceive`, effective I2C address through `_I2CAddress`, MTU setting through `_I2CMax`, time/delay hooks through `_GetMs` and `_DelayMs`
- Artifact coverage: source, declarations, direct callers, dependency macro artifacts, and focused tests inspected

## Behavior

Receives an I2C reply into a caller buffer by repeatedly calling `_I2CReceive(_I2CAddress(), buffer + received, requested, available)`. The first `requested` value is `min(*available, 0xffff, _I2CMax())`; when the caller starts with `*available == 0`, the first hook call is a zero-byte priming query that asks the Notecard how much data is available.

After each successful hook call, the function adds the requested byte count to `received`, resets the timeout window to `CARD_INTRA_TRANSACTION_TIMEOUT_SEC * 1000` after any nonzero read, recomputes the next request from the returned `*available`, and detects end-of-packet when the most recent received byte is `'\n'`. A newline is not sufficient by itself when the hook still reports pending bytes: the loop logs a warning and continues draining until `*available == 0`, or until the caller buffer would overflow.

The loop exits successfully when a complete newline-terminated packet has been read and no bytes remain, or when the next read would exceed the caller buffer. On success and on hook/timeout failure, `*size` is set to the number of bytes actually copied into `buffer`.

## Important Boundaries

- Preconditions and assumptions: `buffer`, `size`, and `available` must be valid; there are no null guards. The I2C receive hook is expected to honor the requested byte count and update `*available`.
- Error/null/empty behavior: hook error logs and returns the hook error with partial byte count in `*size`; timeout while waiting for newline returns `ERRSTR("timeout: transaction incomplete {io}", c_iotimeout)` and logs partial-reply detail only if bytes were received. `timeoutMs == 0` disables the timeout check.
- Ownership and lifetime: caller owns the buffer and in/out counters; this function does not allocate or free memory.
- Concurrency/global state: does not lock I2C itself; callers that need a critical section must wrap it, as `_i2cNoteChunkedReceive` and `_i2cNoteTransaction` do. Reads timing, delay, address, MTU, and receive hook state through finalized macros.
- Portability/platform constraints: platform I2C behavior remains behind `_I2CReceive`, `_I2CAddress`, `_I2CMax`, `_GetMs`, and `_DelayMs`.
- Performance shape: O(number of chunks plus idle polls), with each chunk capped by `0xffff` and the configured I2C maximum. Idle polling sleeps 50 ms only when `delay` is true.
- Security or parsing constraints: newline is the only packet terminator check; buffer-full exit is a non-error overflow handoff that leaves remaining byte count in `*available` for the caller to handle.

## Direct Call Sites

Exact source search found direct production callers:

- `n_i2c.c:140` `_i2cNoteTransaction` receives response JSON into an allocated buffer and grows that buffer while `available` remains nonzero.
- `n_i2c.c:341` `_i2cNoteChunkedReceive` lock-protects and forwards the chunked receive operation for I2C dispatch.

## Role In Callers

This is the private I2C receive engine shared by JSON transactions and binary chunked receive dispatch. Transaction code relies on the buffer-full success boundary to grow the response buffer and continue draining. The dispatch wrapper relies on this function to preserve the adapter-level chunked receive contract.

## Tests Observed

Focused `test/src/_i2cChunkedReceive_test.cpp` covers zero-length caller buffers, receive hook errors, too-small buffers, sufficient buffers with `available` initially zero or nonzero, newline with additional pending data, no-data timeout behavior, partial packet timeout behavior, and whether the `delay` flag controls calls to `NoteDelayMs`.

## Graph Links

- Graph node id: `function:n_i2c.c:365:_i2cChunkedReceive`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_GetMs`, `_I2CMax`, `_I2CReceive`, `_I2CAddress`, `NOTE_C_LOG_ERROR`, `NOTE_C_LOG_WARN`, both `ERRSTR` variants, and `_DelayMs`.

## Evidence Gaps

No unresolved source gaps for this node. `_i2cNoteTransaction` remains a separate preliminary caller artifact because its transmit, query-length, allocation, and response-growth behavior belong to a broader I2C transaction slice.
