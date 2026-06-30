# `_cobsEncodedLength`

## Status

Finalized. Implementation, declaration, and production call sites were inspected from source.

## Identity

- Kind: function
- Definition: `n_cobs.c:196`
- Declaration/interface: `n_lib.h:165`
- Public/exported names: unverified
- Macro aliases/wrappers: none.
- Signature/API shape: `uint32_t _cobsEncodedLength(const uint8_t *ptr, uint32_t length)`
- Visibility: project-internal COBS helper declared in `n_lib.h`.
- Ownership/lifetime role: no allocation or ownership transfer; reads caller buffer.

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: `memchr`.
- Data/type/global dependencies: none beyond COBS block-size constants encoded in the implementation.
- Artifact coverage: direct callers have generated artifacts; caller behavior remains preliminary.

## Behavior

Computes the exact COBS encoded length for a given unencoded byte range, excluding the EOP marker:

1. Starts with one byte for the first COBS code byte.
2. Processes input while `length > 0`.
3. Uses `memchr(ptr, 0, searchLen)` to find the next zero within the bytes that can fit before the current code reaches `0xFF`.
4. Adds non-zero chunk bytes to `encodedLen`.
5. Adds a new code byte when a zero terminates a block or when the COBS code limit is reached.
6. Returns the encoded length without the EOP byte.

## Important Boundaries

- Preconditions and assumptions: `ptr` must be valid for `length` bytes when `length > 0`; there is no NULL guard.
- Error/null/empty behavior: no error channel; for `length == 0`, returns 1 for the initial code byte.
- Ownership and lifetime: reads but does not mutate the input buffer.
- Concurrency/global state: no global state.
- Portability/platform constraints: uses standard C `memchr` and `uint32_t`.
- Performance shape: O(n), optimized by searching runs of non-zero bytes with `memchr`.
- Security or parsing constraints: callers use this as an exact fallback after a cheaper max-length check; no arithmetic overflow guard is present.

## Direct Call Sites

Exact production search `rg -n "\b_cobsEncodedLength\b" -g '!source-research/**' -g '!docs/**'` found:

- Declaration: `n_lib.h:165`.
- Definition: `n_cobs.c:196`.
- Encoder buffer validation fallback: `n_helpers.c:179`.
- Binary transmit buffer validation fallback: `n_helpers.c:548`.

Focused tests exist under `test/src`, but production graph evidence is from the source call sites above.

## Role In Callers

- `NoteBinaryCodecEncode` invokes this only when the constant-time `_cobsEncodedMaxLength` check fails, so the exact O(n) check can accept buffers large enough for the actual data even if smaller than the worst case.
- `NoteBinaryStoreTransmit` uses the same pattern, adding one byte for the newline/EOP transport marker around the exact encoded length.

## Tests Observed

Focused test search found dedicated `_cobsEncodedLength` tests and wrapper tests, but tests were not required to establish the source behavior.

## Graph Links

- Graph node id: `function:n_cobs.c:196:_cobsEncodedLength`
- Structured graph: `inventory/dependency-graph.json`
- Callee artifacts: none.
- Caller artifact candidates: `artifacts/functions/n_helpers_c__L169__NoteBinaryCodecEncode.md`, `artifacts/functions/n_helpers_c__L539__NoteBinaryStoreTransmit.md`.
- Upstream public behavior candidates: public binary codec and binary store APIs declared in `note.h`.

## Evidence Gaps

No unresolved implementation behavior. Caller artifacts remain preliminary, and buffer validity remains a caller precondition when `length > 0`.
