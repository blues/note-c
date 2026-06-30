# `_serialChunkedReceive`

## Status

Finalized. Source-inspected serial chunk receive adapter, finalized timing/serial/logging callees, direct caller and registration sites, and focused tests.

## Identity

- Kind: function
- Definition: `n_serial.c:237`
- Declaration/interface: `n_lib.h:132`
- Public/exported names: none; internal serial adapter function
- Macro aliases/wrappers: registered as the serial `notecardChunkedReceive` callback by `_noteSetActiveInterface`; reached through `_noteChunkedReceive`/`_ChunkedReceive` when serial is active
- Signature/API shape: `const char *_serialChunkedReceive(uint8_t *buffer, uint32_t *size, bool delay, uint32_t timeoutMs, uint32_t *available)`
- Visibility: internal declaration in `n_lib.h`, implementation in `n_serial.c`
- Ownership/lifetime role: fills caller-owned buffer in place; does not allocate or retain pointers

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:243:_GetMs` finalized
  - `macro:n_lib.h:232:_SerialAvailable` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
  - `macro:n_lib.h:244:_DelayMs` finalized
  - `macro:n_lib.h:233:_SerialReceive` finalized
- External/system dependencies: none directly; serial availability and receive behavior are hook-supplied through wrappers
- Data/type/global dependencies: timing and serial hook state through finalized wrappers; error constants `c_iobad` and `c_iotimeout`
- Artifact coverage: source, declaration, direct caller, active-interface registration, finalized callees, overflow/timeout semantics, and focused tests inspected

## Behavior

Initializes `received` to zero and treats `*size` as the caller's available buffer length. If the buffer length is zero, the loop is skipped, `*size` is set to zero, and `NULL` is returned without changing `*available`. Otherwise it waits for `_SerialAvailable()`, optionally delaying 1 ms only while waiting for the first byte when `delay` is true. If `timeoutMs` is nonzero and elapsed time reaches the timeout while waiting, it stores the partial byte count in `*size`, logs a partial-reply error when at least one byte was received, and returns `ERRSTR("timeout: transaction incomplete {io}", c_iotimeout)`.

After each available byte, it switches the timeout window to `CARD_INTRA_TRANSACTION_TIMEOUT_SEC * 1000`, resets `startMs`, reads one byte with `_SerialReceive()`, writes it into `buffer[received++]`, and stops successfully when the byte is newline. If the buffer fills before newline, it sets `*available = 1` and returns `NULL` with `*size` set to the bytes copied. Otherwise it sets `*available = 0`, stores the copied byte count in `*size`, and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: normal callers pass non-null `buffer`, `size`, and `available`; `*size` must describe writable buffer capacity. The function does not validate those pointers before dereference on normal paths.
- Error/null/empty behavior: zero-length buffer returns success with zero bytes and leaves `available` unchanged. Timeout returns an error string containing `{io}` and preserves partial data count. Serial hook errors have no direct channel here; inactive/missing hooks appear as unavailable bytes through `_SerialAvailable`.
- Ownership and lifetime: writes into caller-owned buffer only; no allocation.
- Concurrency/global state: reads timing and serial hook state through wrapper functions; caller is responsible for any surrounding transaction serialization.
- Portability/platform constraints: platform serial behavior stays behind hook wrappers.
- Performance shape: O(bytes copied plus polling time). After first byte, waits greedily for subsequent bytes with no delay calls.
- Security or parsing constraints: copies raw bytes until newline or buffer full; no JSON parsing or content validation.

## Direct Call Sites

Exact source search found direct production references:

- `n_serial.c:98` `_serialNoteTransaction` appends response bytes by repeatedly calling `_serialChunkedReceive`, grows its JSON response buffer when `available` is nonzero, and stops once no more bytes are indicated.
- `n_hooks.c:222` `_noteSetActiveInterface(NOTE_C_INTERFACE_SERIAL)` registers `_serialChunkedReceive` as `notecardChunkedReceive`.
- `n_lib.h:132` declares the internal adapter.

## Role In Callers

`_serialNoteTransaction` treats non-null errors as transaction failure, frees the growing response buffer, logs the error, and returns it. It treats `available` as an overflow/continue signal and allocates additional chunks before calling this helper again. `_noteChunkedReceive` can dispatch indirectly to this helper after the serial active interface installs it.

## Tests Observed

`test/src/_serialChunkedReceive_test.cpp` covers no bytes available timing out, receiving packets smaller than/equal to the output buffer, overflow before newline setting `available` to 1, zero-length output buffer, partial packet timeout, intermittent availability that eventually completes, and the `delay` flag controlling `NoteDelayMs` while waiting for the first byte. `_serialNoteTransaction_test.cpp` fakes this helper to verify caller buffer-growth and error propagation paths. `_noteSetActiveInterface_test.cpp` verifies selecting the serial interface installs `_serialChunkedReceive` into `notecardChunkedReceive`.

## Test Coverage Notes

Focused tests do not cover null pointer arguments or hook-level serial receive failures because the serial receive wrapper has no error-return channel.

## Graph Links

- Graph node id: `function:n_serial.c:237:_serialChunkedReceive`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
