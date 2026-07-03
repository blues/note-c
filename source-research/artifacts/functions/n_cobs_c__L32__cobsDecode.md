# `_cobsDecode`

## Status

Finalized. Source-inspected COBS decode helper.

## Identity

- Kind: function
- Definition: `n_cobs.c:32`
- Declaration/interface: `n_lib.h:163`
- Signature/API shape: `uint32_t _cobsDecode(uint8_t *ptr, uint32_t length, uint8_t eop, uint8_t *dst)`
- Visibility: internal SDK helper declared in `n_lib.h`
- Public/exported names: none
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `memmove`
- Data/type/global dependencies: reads and writes caller-provided byte buffers

## Behavior

Decodes a COBS packet from `ptr` into `dst` and returns the number of decoded bytes written. It tracks the input end as `ptr + length`, inserts restored zero bytes between COBS blocks after the first block, decodes each block length from `(*ptr++) ^ eop`, and stops when that decoded code byte is zero. For each block, it copies `code - 1` payload bytes, clipping the copy length if the encoded code would otherwise read beyond `end`.

When `eop == 0`, it bulk-copies payload bytes with `memmove`, preserving in-place decode safety. When `eop != 0`, it writes `ptr[i] ^ eop` to `dst[i]` in a forward loop. The returned length is `dst - start`.

## Important Boundaries

- Preconditions and assumptions: `ptr` and `dst` must be valid for the passed length and decoded output; callers are responsible for sizing the destination.
- Error/null/empty behavior: no null checks; malformed/truncated block lengths are clipped to available input instead of reporting an explicit error.
- Ownership and lifetime: does not allocate; mutates `dst` and may safely decode in place for normal caller usage.
- Concurrency/global state: no global state.
- Portability/platform constraints: byte-oriented algorithm over `uint8_t`; `memmove` is required for overlapping regions.
- Performance shape: O(encoded length), using bulk copy for the non-XOR path.

## Direct Call Sites

Exact source search found production callers:

- `NoteBinaryCodecDecode` at `n_helpers.c:140`.

Focused test/support callers include `_cobsDecode_test.cpp`, `NoteBinaryCodecDecode_test.cpp` fake declarations, and `NoteBinaryStoreReceive_test.cpp` decode fakes.

## Role In Callers

`NoteBinaryCodecDecode` supplies the Notecard binary newline EOP byte and returns `_cobsDecode`'s decoded length directly. Binary store transmit error paths also reach this helper through `NoteBinaryCodecDecode` to restore the caller's encoded buffer to decoded form.

## Tests Observed

Focused tests in `test/src/_cobsDecode_test.cpp` cover in-place decode, separate-buffer decode, empty payloads, all-zero data, no-zero data, zero at beginning/end, blocks exceeding 254 bytes, and nonzero EOP XOR decoding. `test/src/NoteBinaryCodecDecode_test.cpp` verifies wrapper delegation to `_cobsDecode` for valid parameters.

## Graph Links

- Graph node id: `function:n_cobs.c:32:_cobsDecode`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
