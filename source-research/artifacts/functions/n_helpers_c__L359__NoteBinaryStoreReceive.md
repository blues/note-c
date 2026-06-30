# `NoteBinaryStoreReceive`

## Status

Finalized. Source-inspected public binary-store receive API.

## Identity

- Kind: function
- Definition: `n_helpers.c:359`
- Declaration/interface: `note.h:1663`
- Signature/API shape: `const char * NoteBinaryStoreReceive(uint8_t *buffer, uint32_t bufLen, uint32_t decodedOffset, uint32_t decodedLen)`
- Visibility: public SDK API
- Public/exported names: `NoteBinaryStoreReceive`
- Macro aliases/wrappers: none
- Ownership/lifetime role: fills caller-owned storage in place and returns NULL on success or a const error string on failure

## Dependency Status

- Project-local dependencies:
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `function:n_cobs.c:263:_cobsEncodedMaxLength` finalized
  - `macro:n_lib.h:226:_LockNote` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:1989:JAddIntToObject` finalized
  - `function:n_request.c:428:_noteTransactionShouldLock` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
  - `function:n_str.c:31:strlcpy` finalized
  - `macro:note.h:76:NOTE_C_LOG_DEBUG` finalized
  - `macro:note.h:1014:NOTE_C_LOG_DEBUG` finalized
  - `macro:n_lib.h:239:_ChunkedReceive` finalized
  - `function:n_request.c:756:NoteResetRequired` finalized
  - `function:n_helpers.c:127:NoteBinaryCodecDecode` finalized
  - `function:n_md5.c:301:NoteMD5HashString` finalized
- External/system dependencies: `strncmp`
- Data/type/global dependencies: `NOTE_MD5_HASH_STRING_SIZE`, `CARD_INTRA_TRANSACTION_TIMEOUT_SEC`, JSON type `J`, Notecard lock and active-interface hook state through local aliases

## Behavior

Validates arguments before taking the Notecard lock. It rejects a null buffer, a buffer smaller than `_cobsEncodedMaxLength(decodedLen)`, and `decodedLen == 0`, logging and returning const error strings for those cases.

After validation it locks the Notecard, creates a `card.binary.get` request, adds `"offset"` and `"length"` integer fields, and dispatches the setup request through `_noteTransactionShouldLock(req, false)` because this function already owns the Notecard lock. It deletes the request after dispatch. A null response or a response with `"err"` is treated as initialization failure; an error response's `"err"` field is logged before the response is deleted. On a successful setup response, it copies `"status"` into a fixed MD5 status buffer and deletes the response.

The raw binary payload is read through `_ChunkedReceive(buffer, &bufLen, false, CARD_INTRA_TRANSACTION_TIMEOUT_SEC * 1000, &available)`. The Notecard lock is released immediately after the chunked receive returns. Receive errors are wrapped with `ERRSTR(err, c_err)` and queue `NoteResetRequired`. Any nonzero `available` value is treated as unexpected extra data, logs an error, queues reset, and returns an error.

Successful chunk receive is assumed to include a trailing newline terminator; the function decrements `bufLen` to drop that byte, decodes the COBS payload in place with `NoteBinaryCodecDecode`, and requires the decoded length to equal `decodedLen`. A decoded-length mismatch logs an error, queues reset, and returns an error. On matching length it writes `'\0'` at `buffer[decLen]`, computes the decoded payload MD5 with `NoteMD5HashString`, compares it with the response `"status"` string using `strncmp`, and returns an MD5 mismatch error if the values differ. Success returns NULL.

## Important Boundaries

- Preconditions and assumptions: caller provides a writable buffer large enough for the encoded payload and trailing marker; the chunked receive path returns newline-terminated encoded data on success.
- Error/null/empty behavior: validation errors happen before locking; request allocation and response initialization errors unlock before returning; chunked receive, extra-available, and decoded-length failures queue interface reset; MD5 mismatch does not queue reset in this implementation.
- Ownership and lifetime: caller retains `buffer`; the function overwrites it with encoded bytes, then decoded bytes, then writes a NUL marker after the decoded range. It owns and deletes the request and response `J *` objects it creates or receives.
- Concurrency/global state: holds the Notecard mutex from setup request creation through raw chunk receive, and passes `lockNotecard=false` to the transaction helper to avoid nested lock acquisition.
- Performance shape: O(decoded length) receive/decode/hash work after an O(1) maximum encoded-length check.
- Portability/platform constraints: transport and locking remain behind hook aliases; the function itself is portable C SDK code with no direct platform adapter calls.
- Integrity constraints: validates decoded byte count and MD5 status after receiving binary data, but trusts the setup response to provide the expected status string.

## Direct Call Sites

Exact source search found no direct callers inside production library source beyond the public declaration. HITL helper code calls it in `test/hitl/card.binary/lib/notecard_binary/NotecardBinary.h` to validate transferred card binary content and to retrieve a web response payload. `test/src/NoteBinaryStoreReceive_test.cpp` directly exercises the public API with faked hooks.

## Role In Callers

This is the public API for retrieving a byte range from the Notecard binary store. The HITL validation path uses it repeatedly by chunk offset and compares the decoded bytes against generated source data. The web-response troubleshooting path uses it to retrieve binary-store content, print it, and parse it as JSON.

## Tests Observed

`test/src/NoteBinaryStoreReceive_test.cpp` covers null buffer, insufficient buffer, zero decoded length, request allocation failure, setup response error, chunked receive error, unexpected extra available data, MD5 mismatch, decoded-length mismatch, success with decoded payload contents, and balanced lock/unlock calls. HITL card-binary code exercises receive in validation and parse-helper flows.

## Graph Links

- Graph node id: `function:n_helpers.c:359:NoteBinaryStoreReceive`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
