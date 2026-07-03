# `_cobsEncodedMaxLength`

## Status

Finalized. Implementation, internal declaration, public wrapper relationship, and production call sites were inspected from source.

## Identity

- Kind: function
- Definition: `n_cobs.c:263`
- Declaration/interface: `n_lib.h:166`
- Public/exported names: exposed indirectly through `NoteBinaryCodecMaxEncodedLength` at `n_helpers.c:233`, declared publicly at `note.h:1636`.
- Macro aliases/wrappers: none.
- Signature/API shape: `uint32_t _cobsEncodedMaxLength(uint32_t length)`
- Visibility: project-internal COBS helper declared in `n_lib.h`.
- Ownership/lifetime role: no allocation or ownership transfer; computes a length.

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none.
- Data/type/global dependencies: uses COBS constants in `n_cobs.c`: `COBS_MAX_PACKET_SIZE` and `COBS_EOP_OVERHEAD`.
- Artifact coverage: direct callers have generated artifacts; caller behavior remains preliminary.

## Behavior

Computes the maximum COBS encoded length for an unencoded byte count, including the end-of-packet byte:

1. Computes `codeBytes = (length / COBS_MAX_PACKET_SIZE) + 1`.
2. Returns `length + codeBytes + COBS_EOP_OVERHEAD`.

The formula intentionally uses floor division plus one rather than ceiling division so exact multiples of the 254-byte COBS packet size reserve a final code byte.

## Important Boundaries

- Preconditions and assumptions: accepts any `uint32_t length`; source does not guard arithmetic overflow for extremely large values.
- Error/null/empty behavior: no error channel; for `length == 0`, returns one code byte plus EOP overhead.
- Ownership and lifetime: no memory ownership.
- Concurrency/global state: no global state.
- Portability/platform constraints: fixed-width `uint32_t` arithmetic.
- Performance shape: O(1).
- Security or parsing constraints: callers use this as a conservative buffer-size check for binary codec/store encoding; overflow is not locally checked.

## Direct Call Sites

Exact production search `rg -n "\b_cobsEncodedMaxLength\b" -g '!source-research/**' -g '!docs/**'` found:

- Declaration: `n_lib.h:166`.
- Definition: `n_cobs.c:263`.
- Public max-length wrapper: `n_helpers.c:235`.
- Encoder buffer validation: `n_helpers.c:178`.
- Binary receive buffer validation: `n_helpers.c:368`.
- Binary transmit buffer validation: `n_helpers.c:547`.

Focused tests also cover this helper, but test artifacts are not part of the production graph.

## Role In Callers

- `NoteBinaryCodecMaxEncodedLength` exposes this calculation directly to SDK callers.
- `NoteBinaryCodecEncode`, `NoteBinaryStoreReceive`, and `NoteBinaryStoreTransmit` use the calculation to reject buffers too small to hold newline-terminated encoded binary data.

## Tests Observed

Focused test search found `_cobsEncodedMaxLength` tests and wrapper tests under `test/src`, but source behavior did not require test evidence for finalizing this arithmetic helper.

## Graph Links

- Graph node id: `function:n_cobs.c:263:_cobsEncodedMaxLength`
- Structured graph: `inventory/dependency-graph.json`
- Callee artifacts: none.
- Caller artifact candidates: `artifacts/functions/n_helpers_c__L169__NoteBinaryCodecEncode.md`, `artifacts/functions/n_helpers_c__L233__NoteBinaryCodecMaxEncodedLength.md`, `artifacts/functions/n_helpers_c__L359__NoteBinaryStoreReceive.md`, `artifacts/functions/n_helpers_c__L539__NoteBinaryStoreTransmit.md`.
- Upstream public behavior candidates: public binary codec and binary store APIs declared in `note.h`.

## Evidence Gaps

No unresolved implementation behavior. Caller artifacts remain preliminary, and the arithmetic overflow boundary is recorded because the source does not guard it.
