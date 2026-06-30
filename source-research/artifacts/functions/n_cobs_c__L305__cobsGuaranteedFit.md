# `_cobsGuaranteedFit`

## Status

Finalized. Source-inspected COBS safe decoded-length helper.

## Identity

- Kind: function
- Definition: `n_cobs.c:305`
- Declaration/interface: `n_lib.h:167`
- Signature/API shape: `uint32_t _cobsGuaranteedFit(uint32_t bufLen)`
- Visibility: internal SDK helper declared in `n_lib.h`
- Public/exported names: none
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: constants `COBS_MAX_PACKET_SIZE` and EOP overhead encoded in the formula/comments

## Behavior

Computes the maximum unencoded payload length that is guaranteed to fit after COBS encoding into a buffer of `bufLen` bytes including the EOP byte. For `bufLen <= 2`, it returns `0`. Otherwise it computes `t = bufLen - 2` and returns `t - ((t + 1) / 255)`, the closed-form inverse of the encoder's `u + floor(u / 254) + 2` worst-case requirement.

## Important Boundaries

- Preconditions and assumptions: `bufLen` is a byte count for a fixed buffer that must also hold COBS code bytes and EOP.
- Error/null/empty behavior: sizes 0, 1, and 2 cannot guarantee any decoded payload and return 0.
- Ownership and lifetime: no memory access.
- Concurrency/global state: no global state.
- Performance shape: O(1).
- Security or parsing constraints: used by decode wrappers to reject too-small output buffers before writing.

## Direct Call Sites

Exact source search found production callers:

- `NoteBinaryCodecDecode` at `n_helpers.c:136`.
- `NoteBinaryCodecMaxDecodedLength` at `n_helpers.c:220`.

## Role In Callers

`NoteBinaryCodecDecode` uses this value as its minimum output-buffer requirement for a given encoded length. `NoteBinaryCodecMaxDecodedLength` exposes the same safe capacity calculation to public callers.

## Tests Observed

Focused tests in `test/src/_cobsGuaranteedFit_test.cpp` cover small buffers, zero/one/two-byte boundaries, 255k and 255k+1 boundary cases, customer-reported large capacities, and round trips through `_cobsEncodedMaxLength`. `test/src/NoteBinaryCodecDecode_test.cpp` uses `_cobsGuaranteedFit(encDataLen) - 1` to verify wrapper rejection of undersized output buffers.

## Graph Links

- Graph node id: `function:n_cobs.c:305:_cobsGuaranteedFit`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
