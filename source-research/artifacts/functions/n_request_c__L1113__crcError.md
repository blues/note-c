# `_crcError`

## Status

Finalized. Source-inspected response CRC validator and stripper.

## Identity

- Kind: function
- Definition: `n_request.c:1113`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `bool _crcError(char *json, uint16_t shouldBeSeqno)`
- Visibility: internal to `n_request.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:975:_n_atoh` finalized
  - `function:n_request.c:1019:_crc32` finalized
- External/system dependencies: `strlen`, `strstr`, `memcmp`
- Data/type/global dependencies: `notecardFirmwareSupportsCrc`, CRC field constants

## Behavior

Trims trailing bytes `<= ' '`, rejects invalid object shape as no CRC error, bypasses CRC checks when the raw JSON contains the exact error-field marker, and treats missing CRC as an error only after CRC support has previously been observed. When a tail CRC field is present, sets CRC support true, parses sequence and CRC, mutates the buffer by replacing the CRC field with `}` and NUL, recomputes CRC32 over the stripped JSON, and returns true on sequence or CRC mismatch.

## Important Boundaries

- Preconditions and assumptions: `json` must be non-null and mutable.
- Error/null/empty behavior: malformed JSON shape and explicit error responses return false.
- Ownership and lifetime: mutates caller-owned response buffer in place to remove the CRC field.
- Concurrency/global state: reads/writes `notecardFirmwareSupportsCrc`.
- Performance shape: linear in response length plus CRC over stripped JSON.
- Portability/platform constraints: compiled only when CRC code is included outside low-memory builds.

## Direct Call Sites

Exact source search found `_noteTransactionShouldLock` as the production caller when `_crcAdd` succeeded.

## Role In Callers

A true result makes the transaction free the response, record a CRC I/O error, delay, and retry.

## Tests Observed

`_crcError_test.cpp` covers support negotiation, malformed JSON, exact error bypass, CRC mismatch, sequence mismatch, matching CRC, absent CRC after support, and trailing CRLF. `NoteTransaction_test.cpp` covers transaction CRC retry paths.

## Graph Links

- Graph node id: `function:n_request.c:1113:_crcError`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
