# `NoteBinaryCodecEncode`

## Status

Finalized. Implementation, public declaration, project-local callees, and production call sites were inspected from source.

## Identity

- Kind: function
- Definition: `n_helpers.c:169`
- Declaration/interface: `note.h:1619`
- Public/exported names: `NoteBinaryCodecEncode`
- Macro aliases/wrappers: none.
- Signature/API shape: `uint32_t NoteBinaryCodecEncode(const uint8_t *decData, uint32_t decDataLen, uint8_t *encBuf, uint32_t encBufSize)`
- Visibility: public SDK API.
- Ownership/lifetime role: reads caller-owned decoded data and writes encoded bytes into caller-owned output buffer; does not allocate or take ownership.

## Dependency Status

- Project-local dependencies:
- `macro:note.h:990:NOTE_C_LOG_ERROR` (verified)
- `macro:note.h:75:ERRSTR` / `macro:note.h:78:ERRSTR` (verified conditional variants)
- `function:n_cobs.c:263:_cobsEncodedMaxLength` (verified)
- `function:n_cobs.c:196:_cobsEncodedLength` (verified)
- `function:n_cobs.c:115:_cobsEncode` (verified)
- External/system dependencies: none directly.
- Data/type/global dependencies: `NOTE_C_BINARY_EOP` from `n_helpers.c:81`.
- Artifact coverage: all project-local callees listed above are finalized.

## Behavior

1. Initializes a `result` variable.
2. Returns `0` after logging `"NULL parameter"` if either `decData` or `encBuf` is `NULL`.
3. Checks output capacity with a two-stage policy: first the O(1) worst-case `_cobsEncodedMaxLength(decDataLen)`; only if that fails does it compute exact `_cobsEncodedLength(decData, decDataLen)`.
4. Returns `0` after logging `"output buffer too small"` when the buffer is smaller than both the worst-case and exact encoded sizes.
5. Otherwise calls `_cobsEncode((uint8_t *)decData, decDataLen, NOTE_C_BINARY_EOP, encBuf)` and returns the encoded length.

## Important Boundaries

- Preconditions and assumptions: `encBuf` must be large enough for the encoded data; in-place use is supported only when the decoded data has already been shifted so input and output do not start at the same address.
- Error/null/empty behavior: returns `0` on NULL parameters or insufficient output capacity; no separate error string is returned.
- Ownership and lifetime: no allocation; caller owns both buffers.
- Concurrency/global state: no global state beyond logging/debug hooks.
- Portability/platform constraints: uses internal COBS helper with newline as the binary EOP byte.
- Performance shape: O(1) fast capacity rejection/acceptance for worst case, with O(n) exact length fallback only when the worst-case check fails.
- Security or parsing constraints: casts away `const` when passing to `_cobsEncode`, but source inspection of `_cobsEncode` shows it only reads the input pointer and writes `encBuf`.

## Direct Call Sites

Exact production search `rg -n "\bNoteBinaryCodecEncode\b" -g '!source-research/**' -g '!docs/**'` found:

- Public declaration: `note.h:1619`.
- Definition: `n_helpers.c:169`.
- Production caller: `n_helpers.c:631`, from `NoteBinaryStoreTransmit`.

Focused tests exercise NULL, buffer-size, and call-through behavior but are outside production graph scope.

## Role In Callers

`NoteBinaryStoreTransmit` uses this wrapper after shifting unencoded data toward the end of the caller buffer. It passes the original buffer front as `encBuf`, leaving space for a newline byte that the store transmit path appends separately.

## Tests Observed

Focused test search found wrapper tests in `test/src/NoteBinaryCodecEncode_test.cpp` and binary-store tests using this API. Source behavior and production caller evidence were sufficient for finalization.

## Graph Links

- Graph node id: `function:n_helpers.c:169:NoteBinaryCodecEncode`
- Structured graph: `inventory/dependency-graph.json`
- Callee artifacts: `artifacts/functions/n_cobs_c__L263__cobsEncodedMaxLength.md`, `artifacts/functions/n_cobs_c__L196__cobsEncodedLength.md`, `artifacts/functions/n_cobs_c__L115__cobsEncode.md`, `artifacts/macros/note_h__L75__ERRSTR.md`, `artifacts/macros/note_h__L78__ERRSTR.md`, `artifacts/macros/note_h__L990__NOTE_C_LOG_ERROR.md`.
- Caller artifact candidates: `artifacts/functions/n_helpers_c__L539__NoteBinaryStoreTransmit.md`.
- Upstream public behavior candidates: `NoteBinaryStoreTransmit`.

## Evidence Gaps

No unresolved behavior for this wrapper. Upstream binary store transmit remains preliminary.
