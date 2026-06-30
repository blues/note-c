# `_crc32`

## Status

Finalized. Source-inspected half-byte CRC32 helper.

## Identity

- Kind: function
- Definition: `n_request.c:1019`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `int32_t _crc32(const void *data, size_t length)`
- Visibility: internal to `n_request.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: static 16-entry CRC lookup table `lut`

## Behavior

Initializes CRC to all ones, then processes each input byte in two half-byte lookup steps using `lut`, and returns the bitwise inverse of the final CRC.

## Important Boundaries

- Preconditions and assumptions: `data` must be non-null when `length > 0`.
- Error/null/empty behavior: no error channel; zero length returns the CRC of an empty buffer.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads immutable lookup table only.
- Performance shape: O(length) with two table lookups per byte.
- Portability/platform constraints: uses fixed-width unsigned arithmetic.

## Direct Call Sites

Exact source search found `_crcAdd` and `_crcError` as production callers.

## Role In Callers

`_crcAdd` computes the CRC inserted into outgoing request JSON. `_crcError` recomputes the CRC after stripping the response CRC field.

## Tests Observed

`_crcAdd_test.cpp` and `_crcError_test.cpp` exercise this helper indirectly through request CRC insertion and response CRC validation.

## Graph Links

- Graph node id: `function:n_request.c:1019:_crc32`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
