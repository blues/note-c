# `NoteBinaryStoreTransmit`

## Status

Finalized. Source-inspected public binary-store transmit helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:539`
- Declaration/interface: `note.h:1681`
- Public/exported names: `NoteBinaryStoreTransmit`
- Macro aliases/wrappers: none found
- Signature/API shape: `const char * NoteBinaryStoreTransmit(uint8_t *unencodedData, uint32_t unencodedLen, uint32_t bufLen, uint32_t notecardOffset)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: mutates the caller-provided buffer in place, sends encoded bytes to the Notecard binary store, and returns static/configuration-dependent error strings

## Dependency Status

- Project-local dependencies:
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `function:n_cobs.c:263:_cobsEncodedMaxLength` finalized
  - `function:n_cobs.c:196:_cobsEncodedLength` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:1950:JAddBoolToObject` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `function:n_request.c:940:NoteErrorContains` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson_helpers.c:122:JGetInt` finalized
  - `function:n_md5.c:301:NoteMD5HashString` finalized
  - `function:n_helpers.c:169:NoteBinaryCodecEncode` finalized
  - `macro:n_lib.h:226:_LockNote` finalized
  - `function:n_cjson.c:1989:JAddIntToObject` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_request.c:428:_noteTransactionShouldLock` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
  - `function:n_helpers.c:127:NoteBinaryCodecDecode` finalized
  - `macro:note.h:76:NOTE_C_LOG_DEBUG` finalized
  - `macro:note.h:1014:NOTE_C_LOG_DEBUG` finalized
  - `macro:n_lib.h:240:_ChunkedTransmit` finalized
  - `macro:note.h:998:NOTE_C_LOG_WARN` finalized
- External/system dependencies: libc `memmove`
- Data/type/global dependencies: constants `c_bad`, `c_badbinerr`, `c_err`, `c_mem`, and `NOTE_MD5_HASH_STRING_SIZE`; transitive Notecard, lock, transaction, logging, delay, and transport hooks through callees
- Artifact coverage: source-inspected artifact

## Behavior

Rejects a null `unencodedData` pointer. It then checks whether `bufLen` can hold the COBS-encoded payload plus newline, using the constant-time maximum check first and the data-dependent encoded-length check only if the maximum check fails.

For the initial `card.binary` handshake, it creates a `card.binary` request and adds `"reset": true` when `notecardOffset == 0`. The initial request pointer is not checked locally before adding the reset field, but finalized `JAddBoolToObject` is null-safe and finalized `NoteRequestResponse` returns null for a null request. A null response returns `"unable to issue binary request"`. A response error is swallowed only when it contains `c_badbinerr`; other handshake errors are logged, the response is deleted, and `"unexpected error received during handshake"` is returned.

On handshake success or swallowed `{bad-bin}`, it reads `"length"` and `"max"`, deletes the response, rejects missing/zero `"max"`, rejects offsets that do not equal the returned `"length"`, computes remaining binary-store capacity, and rejects payloads larger than the remaining capacity. It hashes the original unencoded bytes with `NoteMD5HashString`, shifts the unencoded data to the end of the caller buffer with `memmove`, COBS-encodes it into the front of the same buffer with `NoteBinaryCodecEncode`, and appends a newline byte.

It then attempts up to three binary transmissions. For each attempt it locks the Notecard, creates `card.binary.put`, adds `"cobs"`, optional `"offset"`, and `"status"` MD5 fields, calls `_noteTransactionShouldLock(req, false)` while already holding the lock, deletes the request, and requires a non-error response. Initialization failure unlocks and decodes the encoded buffer back to the caller's original bytes before returning. If initialization succeeds, it sends the encoded buffer plus newline through `_ChunkedTransmit`, unlocks, and restores the caller buffer on transmit error.

After each chunk transmit it validates with another `card.binary` request. A null validation response restores the buffer and returns `"unable to validate request"`. A non-`{bad-bin}` validation error restores the buffer and returns `"unexpected error received during confirmation"`. A `{bad-bin}` validation error logs a warning and retries until the third attempt; after retries are exhausted it restores the buffer and returns `"binary data invalid"`. A successful validation deletes the response, leaves the caller buffer encoded/modified, and returns null.

## Important Boundaries

- Preconditions and assumptions: `unencodedData` must be non-null. The caller buffer must be large enough for the encoded payload plus newline and must be writable for `bufLen` bytes.
- Error/null/empty behavior: null data, insufficient buffer, null request/response, missing/zero `"max"`, offset mismatch, store-capacity overflow, binary-put setup failure, chunk transmit failure, validation failure, and repeated `{bad-bin}` confirmation all return non-null error strings. Post-encode error paths attempt to restore the caller buffer by decoding in place.
- Ownership and lifetime: all JSON requests and responses are owned/deleted by this helper or by finalized request wrappers. Returned error strings are not caller-owned. The caller retains ownership of the data buffer, but successful calls leave it modified.
- Concurrency/global state: the binary-put transaction and chunk transmit are protected by `_LockNote`/`_UnlockNote`; the transaction call bypasses nested locking by passing `false` to `_noteTransactionShouldLock`. Hook behavior, active transport, logging, and mutex behavior are global SDK configuration.
- Portability/platform constraints: low-memory builds use compact `ERRSTR` codes; encoded transport behavior depends on configured chunked-transmit hook and active interface.
- Performance shape: one initial request/response, one MD5 pass over the payload, one buffer shift, one COBS encode, up to three binary-put transactions plus chunk sends, and up to three validation request/response cycles.
- Security or parsing constraints: no explicit check that returned `"max"` is greater than or equal to returned `"length"` before `max - len` is converted to `uint32_t`; no explicit encode-failure check before appending newline; request field-add return values are ignored.

## Direct Call Sites

Production-only exact search found only the public declaration in `note.h:1681` and the definition in `n_helpers.c:539`. Focused tests call it directly from `test/src/NoteBinaryStoreTransmit_test.cpp`. A HITL test support wrapper calls it from `test/hitl/card.binary/lib/notecard_binary/NotecardBinary.h:749`.

## Role In Callers

No production repository caller role. Public helper for downstream callers that append sequential chunks to the Notecard binary store, optionally resetting the store when starting at offset zero.

## Tests Observed

Focused tests in `test/src/NoteBinaryStoreTransmit_test.cpp` fake the request/transaction/lock/chunk hooks. Covered cases include null data, initial response null or generic error, missing/zero max, offset mismatch, not enough remaining store space, buffer too small for encoded data plus newline, reset flag on zero offset, binary-put request allocation failure, binary-put transaction failure, chunk transmit failure, validation response null, validation generic error, repeated `{bad-bin}` exhaustion, `{bad-bin}` retry followed by success, buffer restoration on post-encode failures, and balanced lock/unlock counts. Gaps include missing/non-number `"length"` or negative values, `"max" < "length"`, `JAdd*` field-add failure, encode returning zero/failure, exact `card.binary.put` request field values beyond selected tests, and real transport behavior.

## Graph Links

- Graph node id: `function:n_helpers.c:539:NoteBinaryStoreTransmit`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to COBS length helpers, JSON request builders, request/response helpers, response/error helpers, MD5, binary codec encode/decode, Notecard lock/unlock, transaction-with-lock-control, debug/warn/error logging, and chunked transmit.

## Evidence Gaps

No unresolved source gaps for this node.
