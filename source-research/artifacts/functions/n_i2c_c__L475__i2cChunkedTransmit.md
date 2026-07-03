# `_i2cChunkedTransmit`

## Status

Finalized. Source-inspected private I2C chunked transmit loop, dependency macros, direct callers, and focused tests.

## Identity

- Kind: function
- Definition: `n_i2c.c:475`
- Declaration/interface: `n_lib.h:129`
- Public/exported names: none; internal transport helper
- Macro aliases/wrappers: wrapped by `_i2cNoteChunkedTransmit` for lock-protected I2C dispatch
- Signature/API shape: `const char *_i2cChunkedTransmit(const uint8_t *buffer, uint32_t size, bool delay)`
- Visibility: internal SDK transport function declared in `n_lib.h`
- Ownership/lifetime role: reads caller-owned buffer; no allocation or ownership transfer

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:248:_I2CMax` finalized
- `function:n_i2c.c:30:_delayIO` finalized
- `macro:n_lib.h:235:_I2CTransmit` finalized in this slice
- `macro:n_lib.h:247:_I2CAddress` finalized
- `macro:n_lib.h:234:_I2CReset` finalized
- `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
- `macro:n_lib.h:244:_DelayMs` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: I2C hook/address/MTU state through macros; pacing state through `_delayIO` and `_DelayMs`
- Artifact coverage: source, declaration, direct callers, dependency artifacts, and focused tests inspected

## Behavior

Sends the caller buffer in I2C chunks until `size == 0`. Each chunk length is capped first to `0xffff` for the `_I2CTransmit` `uint16_t` size parameter, then to `_I2CMax()`. With `delay == true`, it calls `_delayIO()` before each chunk, `_DelayMs(CARD_REQUEST_I2C_CHUNK_DELAY_MS)` after each chunk, and after more than `CARD_REQUEST_I2C_SEGMENT_MAX_LEN` bytes in a segment it resets the segment counter and delays `CARD_REQUEST_I2C_SEGMENT_DELAY_MS`.

On `_I2CTransmit` error, it calls `_I2CReset(_I2CAddress())` best-effort, logs the original error, and returns that original transmit error. A zero-size input returns `NULL` without transmitting or resetting.

## Important Boundaries

- Preconditions and assumptions: `buffer` must be valid for `size` bytes when `size > 0`; no null guard is present.
- Error/null/empty behavior: zero size is a no-op success; transmit hook error propagates after best-effort reset and logging. Reset failure is ignored in this path.
- Ownership and lifetime: no allocation; passes pointers into the caller buffer to `_I2CTransmit`.
- Concurrency/global state: does not lock I2C itself; `_i2cNoteTransaction` and `_i2cNoteChunkedTransmit` call it under lock.
- Portability/platform constraints: transmit, reset, address, MTU, and delay behavior remain behind SDK hooks/macros.
- Performance shape: O(number of chunks), with chunk size capped by both `0xffff` and `_I2CMax()`, plus optional pacing delays.
- Security or parsing constraints: no parsing; depends on the platform hook honoring chunk length and not mutating the caller buffer despite legacy non-const hook type.

## Direct Call Sites

Exact production callers:

- `n_i2c.c:96` `_i2cNoteTransaction` sends nonzero JSON requests under the transaction I2C lock with `delay=true`.
- `n_i2c.c:459` `_i2cNoteChunkedTransmit` lock-protects and forwards chunked transmit operations for I2C dispatch.

## Role In Callers

This is the private I2C transmit engine shared by JSON transactions and binary chunked transmit dispatch. It is responsible for MTU/chunk/segment pacing and transmit-error reset recovery.

## Tests Observed

Focused `test/src/_i2cChunkedTransmit_test.cpp` covers zero-size no-op, transmit error triggering reset, single-chunk transmit, multi-chunk transmit, and success over a segment-sized buffer. The test contains an intended data equality `memcmp` check that is not asserted, so exact emitted byte sequence is not actually verified by that check.

## Graph Links

- Graph node id: `function:n_i2c.c:475:_i2cChunkedTransmit`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_I2CMax`, `_delayIO`, `_I2CTransmit`, `_I2CAddress`, `_I2CReset`, `NOTE_C_LOG_ERROR`, and `_DelayMs`.

## Evidence Gaps

No unresolved source gaps for this node. Focused tests do not assert exact byte-for-byte transmit payload equality despite an attempted `memcmp`.
