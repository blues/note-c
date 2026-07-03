# `_serialChunkedTransmit`

## Status

Finalized. Source-inspected serial chunk transmit adapter, finalized transmit/timing callees, direct caller and registration sites, and focused tests.

## Identity

- Kind: function
- Definition: `n_serial.c:303`
- Declaration/interface: `n_lib.h:133`
- Public/exported names: none; internal serial adapter function
- Macro aliases/wrappers: registered as the serial `notecardChunkedTransmit` callback by `_noteSetActiveInterface`; reached through `_noteChunkedTransmit`/`_ChunkedTransmit` when serial is active
- Signature/API shape: `const char *_serialChunkedTransmit(const uint8_t *buffer, uint32_t size, bool delay)`
- Visibility: internal declaration in `n_lib.h`, implementation in `n_serial.c`
- Ownership/lifetime role: reads caller-owned buffer and does not retain it

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:231:_SerialTransmit` finalized
  - `macro:n_lib.h:244:_DelayMs` finalized
- External/system dependencies: none directly; serial transmit and delay are hook-supplied through wrappers
- Data/type/global dependencies: serial transmit hook and timing hook state through finalized wrappers; compile-time guard on `CARD_REQUEST_SERIAL_SEGMENT_MAX_LEN`
- Artifact coverage: source, declaration, direct caller, active-interface registration, finalized callees, segmentation behavior, and focused tests inspected

## Behavior

Splits `buffer[0:size]` into segments no larger than `CARD_REQUEST_SERIAL_SEGMENT_MAX_LEN`. For each segment, calls `_SerialTransmit(&buffer[segOff], segLen, false)`, advances the offset, and subtracts the segment length from the remaining byte count. It skips the trailing delay after the final segment. Between segments, when `delay` is true, it calls `_DelayMs(CARD_REQUEST_SERIAL_SEGMENT_DELAY_MS)`. Size zero performs no transmit calls and returns `NULL`. The function returns `NULL` unconditionally on all runtime paths.

## Important Boundaries

- Preconditions and assumptions: normal callers pass a valid `buffer` whenever `size > 0`. The function does not validate `buffer` before indexing it on nonzero-size paths.
- Error/null/empty behavior: no runtime error reporting; serial transmit hook failures have no channel through `_SerialTransmit`. Zero-length input returns success without transmitting.
- Ownership and lifetime: reads from caller-owned memory only; no allocation and no retention.
- Concurrency/global state: serial transmit and delay behavior use global hook state through wrappers; caller is responsible for surrounding transaction serialization.
- Portability/platform constraints: compile-time error if `CARD_REQUEST_SERIAL_SEGMENT_MAX_LEN` exceeds `SIZE_MAX`; platform serial output remains behind the transmit hook.
- Performance shape: O(size) transmit work, with optional fixed delay between segments.
- Security or parsing constraints: sends raw bytes; no parsing or validation.

## Direct Call Sites

Exact source search found direct production references:

- `n_serial.c:48` `_serialNoteTransaction` strips newline/optional carriage return from the JSON request, sends the request body through `_serialChunkedTransmit(..., true)`, and then sends `"\r\n"` through `_SerialTransmit`.
- `n_hooks.c:223` `_noteSetActiveInterface(NOTE_C_INTERFACE_SERIAL)` registers `_serialChunkedTransmit` as `notecardChunkedTransmit`.
- `n_lib.h:133` declares the internal adapter.

## Role In Callers

`_serialNoteTransaction` treats a non-null return from this helper as transmit failure, logs it, and returns it. In the current implementation this helper never returns an error, so serial transmit errors can only be represented below the hook layer, not through this adapter.

## Tests Observed

`test/src/_serialChunkedTransmit_test.cpp` verifies zero-length input returns no error, buffers smaller than/equal to the segment limit are transmitted once and verbatim, larger buffers are split across multiple `_noteSerialTransmit` calls without altering bytes, and the `delay` flag controls whether `NoteDelayMs` is called between segments. `_serialNoteTransaction_test.cpp` fakes this helper to verify caller error propagation. `_noteSetActiveInterface_test.cpp` verifies selecting the serial interface installs `_serialChunkedTransmit` into `notecardChunkedTransmit`.

## Test Coverage Notes

Focused tests do not cover null `buffer` with nonzero size or platform transmit-hook failure because `_SerialTransmit` has no error-return channel.

## Graph Links

- Graph node id: `function:n_serial.c:303:_serialChunkedTransmit`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
