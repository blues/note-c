# `NoteBinaryCodecDecode`

## Status

Finalized. Source-inspected public binary decode wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:127`
- Declaration/interface: `note.h:1607`
- Signature/API shape: `uint32_t NoteBinaryCodecDecode(const uint8_t *encData, uint32_t encDataLen, uint8_t *decBuf, uint32_t decBufSize)`
- Visibility: public SDK function declared in `note.h`
- Public/exported names: `NoteBinaryCodecDecode`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cobs.c:305:_cobsGuaranteedFit` finalized
  - `function:n_cobs.c:32:_cobsDecode` finalized
  - `macro:note.h:75:ERRSTR` finalized low-memory variant
  - `macro:note.h:78:ERRSTR` finalized normal variant
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: reads static `NOTE_C_BINARY_EOP` newline constant at `n_helpers.c:81`

## Behavior

Validates inputs before decoding Notecard binary data. If `encData` or `decBuf` is `NULL`, it logs `ERRSTR("NULL parameter", c_err)` and returns `0`. If `decBufSize` is less than `_cobsGuaranteedFit(encDataLen)`, it logs `ERRSTR("output buffer too small", c_err)` and returns `0`. Otherwise it delegates to `_cobsDecode((uint8_t *)encData, encDataLen, NOTE_C_BINARY_EOP, decBuf)` and returns the decoded byte count unchanged.

## Important Boundaries

- Preconditions and assumptions: valid non-null buffers must be large enough for `_cobsGuaranteedFit(encDataLen)`; in-place decode is allowed by passing the same address for `encData` and `decBuf`.
- Error/null/empty behavior: wrapper-level validation failures return 0 and log errors; decode-level malformed COBS input is not converted to a distinct error.
- Ownership and lifetime: does not allocate; mutates `decBuf`; casts away const on `encData` only for the lower helper's in-place-capable signature.
- Concurrency/global state: no mutable global state; logging uses finalized logging macro behavior.
- Portability/platform constraints: relies on byte-level COBS helper and newline EOP constant.
- Performance shape: O(encoded length) after O(1) validation.

## Direct Call Sites

Exact source search found production callers:

- Active receive path at `n_helpers.c:450`, decoding the wire bytes in place after stripping the terminating newline.
- Binary transmit error and retry paths at `n_helpers.c:668`, `n_helpers.c:677`, `n_helpers.c:691`, `n_helpers.c:700`, `n_helpers.c:717`, and `n_helpers.c:724`, restoring the caller's encoded buffer to decoded form on failures.

## Role In Callers

`NoteBinaryStoreReceive` uses `NoteBinaryCodecDecode` to convert the received COBS payload in place and then checks the returned length against the expected decoded length. `NoteBinaryStoreTransmit` uses it as a recovery step after encoding the caller's buffer for transmission, so failed transmissions can leave the buffer in decoded form for retry by the caller.

## Tests Observed

Focused tests in `test/src/NoteBinaryCodecDecode_test.cpp` verify null parameter rejection, undersized buffer rejection using `_cobsGuaranteedFit`, delegation to `_cobsDecode`, argument forwarding for `encData`, `encDataLen`, and `decBuf`, and return-value passthrough. Focused `_cobsDecode` and `_cobsGuaranteedFit` tests cover the lower helper behavior.

## Graph Links

- Graph node id: `function:n_helpers.c:127:NoteBinaryCodecDecode`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_cobsGuaranteedFit`, `_cobsDecode`, `ERRSTR`, and `NOTE_C_LOG_ERROR`.

## Evidence Gaps

No unresolved source gaps for this node.
