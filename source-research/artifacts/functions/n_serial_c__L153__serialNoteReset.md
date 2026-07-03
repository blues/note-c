# `_serialNoteReset`

## Status

Finalized. Source-inspected serial reset adapter.

## Identity

- Kind: function
- Definition: `n_serial.c:153`
- Declaration/interface: `n_lib.h:127`
- Public/exported names: `_serialNoteReset` inside the note-c internal transport surface
- Macro aliases/wrappers: no macro alias found; selected dynamically through `notecardReset`
- Signature/API shape: `bool _serialNoteReset(void)`
- Visibility: internal adapter entry point with prototype in `n_lib.h`
- Ownership/lifetime role: performs serial reset/resynchronization through configured serial hooks

## Dependency Status

- Project-local dependencies:
  - `macro:note.h:76:NOTE_C_LOG_DEBUG` finalized
  - `macro:note.h:1014:NOTE_C_LOG_DEBUG` finalized
  - `macro:n_lib.h:244:_DelayMs` finalized
  - `macro:n_lib.h:230:_SerialReset` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
  - `macro:n_lib.h:231:_SerialTransmit` finalized
  - `macro:n_lib.h:243:_GetMs` finalized
  - `macro:n_lib.h:232:_SerialAvailable` finalized
  - `macro:n_lib.h:233:_SerialReceive` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads `CARD_REQUEST_SERIAL_SEGMENT_DELAY_MS`, `CARD_RESET_SYNC_RETRIES`, and `CARD_RESET_DRAIN_MS`; hook-backed macros reach serial reset/transmit/available/receive, time/delay, and logging hooks
- Artifact coverage: source-inspected artifact with verified direct call edges

## Behavior

Logs reset start, delays for `CARD_REQUEST_SERIAL_SEGMENT_DELAY_MS`, and calls `_SerialReset`. If that reset fails, logs an error and returns false. It then attempts up to `CARD_RESET_SYNC_RETRIES` serial resynchronization passes. Each pass transmits a bare line-feed byte, drains serial data for at least `CARD_RESET_DRAIN_MS`, and treats the Notecard as ready only when some data was seen and every received byte was `\n` or `\r`.

If non-control data is seen, it resets the drain timer and records the pass as unsynchronized. If no data or non-control data prevents success, it logs an error, waits another drain interval, calls `_SerialReset`, and retries. A reset failure during retry returns false immediately. The function returns true only after a control-only response pass; otherwise it returns false after retries are exhausted.

## Important Boundaries

- Preconditions and assumptions: serial hooks must have been installed before the active reset path selects this adapter.
- Error/null/empty behavior: initial or retry `_SerialReset` failure returns false; no serial availability across retries returns false; non-control response bytes cause retry and can ultimately return false.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: no local serial lock is taken here; public `NoteReset` reaches reset through a higher-level Notecard lock.
- Portability/platform constraints: all serial behavior remains behind hook-backed serial macros.
- Performance shape: bounded retry loop; each failed sync pass drains for `CARD_RESET_DRAIN_MS`, waits another drain interval, and resets before retrying.
- Security or parsing constraints: sync classification is byte-level only, accepting only CR/LF response bytes.

## Direct Call Sites

Exact source search found the prototype in `n_lib.h:127`, the definition in `n_serial.c:153`, assignment to `notecardReset` by `_noteSetActiveInterface` in `n_hooks.c:220`, and focused tests. No direct production call was found.

## Role In Callers

`_noteSetActiveInterface` installs this adapter into the active reset function pointer for `NOTE_C_INTERFACE_SERIAL`; `_noteHardReset` dispatches to it through `notecardReset`. Public `NoteReset` and transaction-reset paths therefore reach this function indirectly after serial has been selected.

## Tests Observed

Focused tests in `test/src/_serialNoteReset_test.cpp` cover initial reset failure, unavailable serial data with retries, non-control character retry, retry reset failure, control-character success, and millisecond counter overflow. `_noteSetActiveInterface_test.cpp` covers registration of this function as the serial reset target. Gaps: exact transmitted LF byte, CR-only success, and delay durations are not directly asserted.

## Graph Links

- Graph node id: `function:n_serial.c:153:_serialNoteReset`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing call edges to serial reset/transmit/available/receive, time/delay, and logging/error helpers.
- Verified incoming dynamic edges from `_noteSetActiveInterface` (`assigns_function_pointer`) and `_noteHardReset` (`dispatches_via_pointer`).

## Evidence Gaps

No unresolved source gaps for this node.
