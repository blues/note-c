# `_serialNoteTransaction`

## Status

Finalized. Source-inspected serial JSON transaction adapter.

## Identity

- Kind: function
- Definition: `n_serial.c:35`
- Declaration/interface: `n_lib.h:126`
- Public/exported names: `_serialNoteTransaction` inside the note-c internal transport surface
- Macro aliases/wrappers: no macro alias found; selected dynamically through `notecardTransaction`
- Signature/API shape: `const char *_serialNoteTransaction(const char *request, size_t reqLen, char **response, uint32_t timeoutMs)`
- Visibility: internal adapter entry point with prototype in `n_lib.h`
- Ownership/lifetime role: sends an optional serial request and optionally returns a heap-allocated response buffer through `*response`

## Dependency Status

- Project-local dependencies:
  - `function:n_serial.c:303:_serialChunkedTransmit` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `macro:n_lib.h:231:_SerialTransmit` finalized
  - `macro:n_lib.h:243:_GetMs` finalized
  - `macro:n_lib.h:232:_SerialAvailable` finalized
  - `macro:note.h:76:NOTE_C_LOG_DEBUG` finalized
  - `macro:note.h:1014:NOTE_C_LOG_DEBUG` finalized
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
  - `macro:n_lib.h:244:_DelayMs` finalized
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_serial.c:237:_serialChunkedReceive` finalized
  - `macro:n_lib.h:242:_Free` finalized
- External/system dependencies: `memcpy`
- Data/type/global dependencies: reads `ALLOC_CHUNK`, `CARD_INTRA_TRANSACTION_TIMEOUT_SEC`, `c_newline_len`, and `cardTurboIO`; hook-backed macros reach serial transmit/available/receive, malloc/free, time/delay, and logging hooks
- Artifact coverage: source-inspected artifact with verified direct call edges

## Behavior

If `reqLen > 0`, treats the trailing byte as the request newline, removes it from the transmit length, removes a preceding carriage return when present, transmits the remaining body through `_serialChunkedTransmit`, then sends canonical `\r\n` with `_SerialTransmit`. Errors from `_serialChunkedTransmit` are logged and returned. The final `_SerialTransmit` has a void-return hook boundary, so hook failure cannot be reported here.

If `response == NULL`, returns success after the optional transmit. Otherwise it waits until `_SerialAvailable` reports data or `timeoutMs` elapses, using unsigned elapsed-time arithmetic that tolerates millisecond counter wrap. While waiting, it delays 10 ms when turbo I/O is disabled. On timeout it returns a transaction timeout error. It then allocates an `ALLOC_CHUNK + 1` buffer, receives one or more chunks with `_serialChunkedReceive`, grows the buffer in `ALLOC_CHUNK` multiples when more data remains, null-terminates the response, assigns it to `*response`, and returns success.

## Important Boundaries

- Preconditions and assumptions: when `reqLen > 0`, `request` must point to at least `reqLen` bytes and the last byte is expected to be newline framing; non-null `response` must point to writable `char *` storage.
- Error/null/empty behavior: zero-length requests skip transmit and CRLF append; null `response` skips receive work; timeout, allocation, chunk transmit, and chunk receive errors return an error string and leave `*response` unchanged.
- Ownership and lifetime: successful response capture transfers the allocated response buffer to the caller; receive and growth failures free temporary buffers before returning.
- Concurrency/global state: no local serial lock is taken here; callers reach this through higher-level transaction locking when required.
- Portability/platform constraints: serial I/O stays behind hook-backed macros and serial chunk helpers.
- Performance shape: optional request transmit plus CRLF append; response wait loop polls availability; receive buffer starts at `ALLOC_CHUNK` and grows by rounded chunks.
- Security or parsing constraints: returns raw null-terminated response bytes and does not parse or validate JSON.

## Direct Call Sites

Exact source search found the prototype in `n_lib.h:126`, the definition in `n_serial.c:35`, assignment to `notecardTransaction` by `_noteSetActiveInterface` in `n_hooks.c:221`, and focused tests. No direct production call was found.

## Role In Callers

`_noteSetActiveInterface` installs this adapter into the active transaction function pointer for `NOTE_C_INTERFACE_SERIAL`; `_noteJSONTransaction` dispatches to it through `notecardTransaction`. Public request flows therefore reach this function indirectly after serial has been selected.

## Tests Observed

Focused tests in `test/src/_serialNoteTransaction_test.cpp` cover request transmit plus CRLF append, chunk transmit errors, initial allocation failure, timeout including millisecond wrap, receive errors, empty response, single and multiple receive chunks, growth allocation failure, zero-length requests, and null zero-length requests. `_noteSetActiveInterface_test.cpp` covers registration of this function as the serial transaction target. Gaps: newline-only `reqLen == 1`, carriage-return stripping, turbo-delay suppression, and final CRLF hook failure behavior are not directly asserted.

## Graph Links

- Graph node id: `function:n_serial.c:35:_serialNoteTransaction`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing call edges to serial chunk transmit/receive, serial hook macros, time/delay, allocation/free, and logging/error helpers.
- Verified incoming dynamic edges from `_noteSetActiveInterface` (`assigns_function_pointer`) and `_noteJSONTransaction` (`dispatches_via_pointer`).

## Evidence Gaps

No unresolved source gaps for this node.
