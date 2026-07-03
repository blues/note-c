# `_i2cNoteQueryLength`

## Status

Finalized. Source-inspected I2C priming length-query loop, direct caller, and focused tests.

## Identity

- Kind: function
- Definition: `n_i2c.c:48`
- Declaration/interface: forward declaration at `n_i2c.c:20`; exposed to tests through `test/include/test_static.h:23`
- Public/exported names: none
- Macro aliases/wrappers: none
- Signature/API shape: `NOTE_C_STATIC const char * _i2cNoteQueryLength(uint32_t *available, uint32_t timeoutMs)`
- Visibility: internal I2C transport helper
- Ownership/lifetime role: writes available-byte count through caller-owned pointer

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:243:_GetMs` finalized
  - `macro:n_lib.h:244:_DelayMs` finalized
  - `macro:n_lib.h:236:_I2CReceive` finalized
  - `macro:n_lib.h:247:_I2CAddress` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
- External/system dependencies: none
- Data/type/global dependencies: active I2C receive/address hooks behind macros; timeout string constant `c_iotimeout`
- Artifact coverage: source, forward declaration, direct caller, protocol shape, and focused tests inspected

## Behavior

Creates a local one-byte `dummy_buffer` initialized to zero, then loops while `*available` is zero. The loop captures `startMs = _GetMs()` once before the first receive attempt and delays 50 ms after each unsuccessful iteration through the `for` increment expression.

Each iteration performs a priming receive: `_I2CReceive(_I2CAddress(), &dummy_buffer, 0, available)`. The receive size is zero, so this call asks the I2C receive hook to report the number of available bytes without copying payload bytes. If `_I2CReceive` returns an error string, the function logs it and returns that same pointer. If `timeoutMs` is nonzero and `_GetMs() - startMs >= timeoutMs`, it logs and returns `ERRSTR("timeout: no response from Notecard {io}", c_iotimeout)`. When `*available` becomes nonzero, the loop exits and the function returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `available` must be non-null. The active I2C receive hook must honor a zero-length receive as a query for available bytes.
- Error/null/empty behavior: receive errors are propagated unchanged. A zero `timeoutMs` disables the timeout branch; if availability never changes, the loop can run indefinitely.
- Ownership and lifetime: no allocation; mutates caller-owned `*available`.
- Concurrency/global state: uses global I2C hook/address state and platform time/delay hooks through macros.
- Portability/platform constraints: default I2C address comes from `NoteI2CAddress` when no explicit address is configured. Time behavior depends on `_GetMs`; delay cadence depends on `_DelayMs`.
- Performance shape: polling loop with one zero-byte I2C receive per iteration and 50 ms delay between unsuccessful attempts.
- Security or parsing constraints: no payload parsing; this only discovers the available byte count before a later read.

## Direct Call Sites

Exact production caller found by source search:

- `n_i2c.c:117` `_i2cNoteTransaction` initializes `available = 0`, calls `_i2cNoteQueryLength(&available, timeoutMs)`, logs `"failed to query Notecard"` on error, unlocks I2C, and returns the query error.

## Role In Callers

`_i2cNoteTransaction` uses this as the first response-read phase after a request transmit and `_delayIO`. A successful return guarantees `available` is nonzero by the loop condition, which lets the caller size its JSON response buffer before chunked receive.

## Tests Observed

`test/src/_i2cNoteQueryLength_test.cpp` covers receive hook error propagation, timeout when bytes never become available, immediate availability, and delayed availability after two zero-available receive attempts. `_i2cNoteTransaction_test.cpp` fakes this helper to verify caller behavior for query errors and notes that a successful return with zero available is impossible by the loop condition.

## Test Coverage Notes

Focused tests do not assert exact `_I2CReceive` arguments, `timeoutMs == 0`, the 50 ms delay call count, initial `*available != 0` skip behavior, or behavior of a concrete platform I2C hook implementation.

## Graph Links

- Graph node id: `function:n_i2c.c:48:_i2cNoteQueryLength`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
