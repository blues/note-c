# Transport Implementations Worker Report

## Scope

- Worker: `019f1417-b041-7841-a250-b5a6286f4c95`
- Report status: merged as source-backed inspected evidence
- Source policy: source-only; no tests or source-research summaries
- Primary source inspected: `n_i2c.c:1-513`, `n_serial.c:1-335`, `n_lib.h:1-266`, `note.h:1-100`, `note.h:150-280`, `note.h:640-890`, `note.h:980-1020`, `note.h:1060-1110`
- Focused support inspected: `n_hooks.c`, `n_request.c`, `n_helpers.c`, `n_cobs.c`, `n_const.c`, `CMakeLists.txt`

## I2C Path

- `_i2cNoteTransaction` locks I2C, transmits nonzero requests through `_i2cChunkedTransmit`, exits early if `response == NULL`, delays via `_delayIO`, then queries response length with `_i2cNoteQueryLength` before allocating a rounded `ALLOC_CHUNK` buffer and reading until `available == 0`.
- `_i2cNoteQueryLength` repeatedly calls `_I2CReceive(_I2CAddress(), dummy, 0, available)` as a priming query, sleeping 50 ms between tries, until bytes are available or `timeoutMs` expires with a timeout error.
- `_i2cChunkedReceive` requests `min(*available, 0xffff, _I2CMax())`, drains even after newline if the Notecard still reports bytes, warns on newline-before-drain-complete, resets timeout to 1 s after any nonzero request, and returns an incomplete transaction timeout on idle timeout.
- `_i2cChunkedTransmit` chunks by `min(size, 0xffff, _I2CMax())`, calls `_delayIO` before each chunk when requested, calls `_I2CTransmit`, resets I2C on transmit error, delays 20 ms per chunk and 250 ms after the accumulated segment exceeds 250 bytes.
- `_i2cNoteReset` locks I2C, delays 250 ms, calls `_I2CReset`, then up to 10 times sends bare LF, drains for 500 ms, and succeeds only if received data is present and all bytes are CR/LF. No response triggers another `_I2CReset`; non-control data warns and retries.

## Serial Path

- `_serialNoteTransaction` strips trailing LF and optional CR, transmits body segments, then sends CRLF with flush. It returns immediately for `response == NULL`, otherwise waits for `_SerialAvailable()` with optional timeout and 10 ms sleeps unless `cardTurboIO` is true.
- Serial receive allocates one `ALLOC_CHUNK`, calls `_serialChunkedReceive`, grows by rounded `ALLOC_CHUNK` when `available` is set, and returns the newline-terminated response buffer.
- `_serialChunkedReceive` reads one byte at a time until newline or buffer overflow. On overflow before newline it sets `*available = 1` as a synthetic continuation signal. It delays only while waiting for the first byte, then spins greedily; after any byte it resets timeout to the 1 s intra-transaction timeout.
- `_serialChunkedTransmit` segments at 250 bytes, calls `_SerialTransmit(..., flush=false)`, and delays 250 ms between segments. Serial transmit hooks are `void`, so this path cannot surface platform transmit errors.
- `_serialNoteReset` delays 250 ms, calls `_SerialReset`, sends bare LF up to 10 times, drains for 500 ms, and succeeds only on CR/LF-only data. Failure logs unrecognized data or no response, delays 500 ms, resets serial, and retries.

## Call And Hook Evidence

- Exact all-source searches verified dispatcher edges in `_noteSetActiveInterface`: serial maps to `_serialNoteReset`, `_serialNoteTransaction`, `_serialChunkedReceive`, `_serialChunkedTransmit`; I2C maps to `_i2cNoteReset`, `_i2cNoteTransaction`, `_i2cNoteChunkedReceive`, `_i2cNoteChunkedTransmit`.
- Public request callers go through `_Transaction -> _noteJSONTransaction` in `n_request.c` transaction and ping paths.
- Binary helpers are the only production `_ChunkedReceive`/`_ChunkedTransmit` callers found, at `n_helpers.c:420` and `n_helpers.c:683`.
- `n_lib.h` macro-dispatches transport hook names: `_I2CReceive -> _noteI2CReceive`, `_SerialTransmit -> _noteSerialTransmit`, `_I2CMax -> NoteI2CMax`, and related aliases.
- Dynamic call edges are platform hooks. `_noteSerial*` calls `hookSerial*` only when active interface is serial; `_noteI2C*` calls `hookI2C*` only when active interface is I2C. Actual hook implementations are external and registered through `NoteSetFnSerial*` and `NoteSetFnI2C*`; no production in-repo setter caller was found in non-doc source.
- COBS is not used inside `n_i2c.c` or `n_serial.c`. Binary helpers encode/decode COBS using EOP `'\n'`, then send/receive raw bytes through chunked transport.

## State And Boundaries

- `cardTurboIO` defaults false and suppresses only selected delay loops.
- I2C address/MTU globals are `i2cAddress` and `i2cMax`, defaulting through `NOTE_I2C_ADDR_DEFAULT` (`0x17`) and `NOTE_I2C_MTU_DEFAULT` (`30`), clamped to max `253`.
- `ERRSTR(x,y)` returns verbose strings normally, but low-memory builds return terse constants such as `c_iotimeout = "timeout {io}"`, `c_iobad = "bad {io}"`, and `c_mem = "mem"`.
- Transports do not validate `request` when `reqLen > 0`.
- Serial has a potential underflow if called with `reqLen == 1` before checking `request[reqLen - 1]`.
- `timeoutMs == 0` disables timeout checks.
- If timing hooks are unset, `NoteGetMs()` returns 0 and `NoteDelayMs()` is a no-op.

## Recommended Claims

- Record active-interface function-pointer dispatch, I2C priming query, I2C drain-after-newline behavior, serial CRLF normalization, reset sync loops, timeout layering, chunk/MTU limits, turbo delay suppression, and COBS framing being owned by binary helpers rather than transports.
- Model hook calls as `possible_dynamic_call` or `dispatches_via_hook` edges to platform callbacks, with uncertainty noting implementations are downstream-supplied.
