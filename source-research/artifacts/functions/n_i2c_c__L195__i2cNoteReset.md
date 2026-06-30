# `_i2cNoteReset`

## Status

Finalized. Source-inspected I2C reset/resync adapter endpoint, dependency macros, dispatch role, and focused tests.

## Identity

- Kind: function
- Definition: `n_i2c.c:195`
- Declaration/interface: `n_lib.h:125`
- Public/exported names: none; internal adapter target selected by `_noteSetActiveInterface`
- Macro aliases/wrappers: selected as `notecardReset` for `NOTE_C_INTERFACE_I2C`
- Signature/API shape: `bool _i2cNoteReset(void)`
- Visibility: internal SDK transport function declared in `n_lib.h`
- Ownership/lifetime role: no allocation; drains reset bytes into stack buffers

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:245:_LockI2C` finalized
- `macro:note.h:76:NOTE_C_LOG_DEBUG` finalized
- `macro:note.h:1014:NOTE_C_LOG_DEBUG` finalized
- `macro:n_lib.h:244:_DelayMs` finalized
- `macro:n_lib.h:234:_I2CReset` finalized
- `macro:n_lib.h:247:_I2CAddress` finalized
- `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
- `macro:note.h:75:ERRSTR` finalized
- `macro:note.h:78:ERRSTR` finalized
- `macro:n_lib.h:246:_UnlockI2C` finalized
- `function:n_i2c.c:30:_delayIO` finalized
- `macro:n_lib.h:235:_I2CTransmit` finalized in this slice
- `macro:n_lib.h:243:_GetMs` finalized
- `macro:n_lib.h:248:_I2CMax` finalized
- `macro:n_lib.h:236:_I2CReceive` finalized
- `macro:note.h:998:NOTE_C_LOG_WARN` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: I2C hook/address/MTU/timing/logging state through dependencies
- Artifact coverage: implementation, declaration, dispatch assignment, hard-reset dispatcher role, dependency artifacts, and focused tests inspected

## Behavior

Locks I2C, logs reset start, delays `CARD_REQUEST_I2C_SEGMENT_DELAY_MS`, then calls `_I2CReset(_I2CAddress())`. If the initial reset fails, it logs, unlocks, and returns false immediately. On success it calls `_delayIO()` and enters a bounded reset-sync retry loop.

Each retry sends a stack `'\n'` byte through `_I2CTransmit(_I2CAddress(), lf, 1)`. Transmit errors are logged, followed by `CARD_REQUEST_I2C_NACK_WAIT_MS`, and the loop retries. Successful transmit is followed by a segment delay and a drain window lasting `CARD_RESET_DRAIN_MS`, using `_I2CReceive` first with zero length to query available bytes, then with chunk lengths capped by stack buffer size and `_I2CMax()`. During the drain it records whether any bytes were seen and whether any byte was not `'\r'` or `'\n'`; non-control bytes reset the drain timer.

If the drain saw only carriage returns/newlines, the reset succeeds and exits the retry loop. If it saw other data, the function logs a warning and retries. If it saw nothing, it logs no response, calls `_I2CReset(_I2CAddress())` again, delays on success, and retries; a reset failure breaks the loop. The function unlocks before returning the final `notecardReady` state.

## Important Boundaries

- Preconditions and assumptions: active I2C hooks should be configured; `_I2CReset`, `_I2CTransmit`, and `_I2CReceive` define platform success/failure.
- Error/null/empty behavior: initial reset failure returns false immediately; transmit/receive errors keep retrying until retry exhaustion; no drained response triggers an extra reset attempt; non-control drained bytes cause retry; only clean `\r`/`\n` drain marks success.
- Ownership and lifetime: uses stack buffers only; no allocation or ownership transfer.
- Concurrency/global state: holds the I2C lock across the whole reset/resync sequence and unlocks on all observed return paths.
- Portability/platform constraints: reset, transmit, receive, address, MTU, timing, and locking are hook-backed.
- Performance shape: bounded by `CARD_RESET_SYNC_RETRIES`, each with a `CARD_RESET_DRAIN_MS` drain loop plus configured delays.
- Security or parsing constraints: reset success accepts only control-line bytes from the drain; arbitrary data is treated as unsynchronized noise and retried.

## Direct Call Sites

Exact source search found production dispatch assignment:

- `n_hooks.c:226` `_noteSetActiveInterface` assigns `notecardReset = _i2cNoteReset` for `NOTE_C_INTERFACE_I2C`.

The selected reset function pointer is called by `n_hooks.c:1020` `_noteHardReset`; `_Reset` aliases `_noteHardReset`, and higher-level request/ping reset paths reach I2C reset through that active-interface dispatcher.

## Role In Callers

This is the I2C active-interface hard-reset endpoint. It converts the generic reset dispatch into a bus-level reset plus Notecard newline resynchronization sequence.

## Tests Observed

Focused `test/src/_i2cNoteReset_test.cpp` covers initial delay, locking/unlocking, reset address forwarding and failure, post-reset delay, newline transmit address/byte/length, transmit failure retry and NACK delay, receive query/drain behavior, chunk-size caps, drain duration, protocol-error retry, no-response retry reset, retry-reset failure, clean CR/LF success, and lock balance. `_noteSetActiveInterface_test.cpp` verifies I2C dispatch pointer installation, and `_noteHardReset_test.cpp` covers the generic reset dispatcher.

## Graph Links

- Graph node id: `function:n_i2c.c:195:_i2cNoteReset`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to lock/unlock, reset/transmit/receive/address/MTU/time/delay/logging helpers and macros.

## Evidence Gaps

No unresolved source gaps for this node.
