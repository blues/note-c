# `_cobsEncode`

## Status

Finalized. Implementation, declaration, and production call site were inspected from source.

## Identity

- Kind: function
- Definition: `n_cobs.c:115`
- Declaration/interface: `n_lib.h:164`
- Public/exported names: unverified
- Macro aliases/wrappers: none.
- Signature/API shape: `uint32_t _cobsEncode(uint8_t *ptr, uint32_t length, uint8_t eop, uint8_t *dst)`
- Visibility: project-internal COBS helper declared in `n_lib.h`.
- Ownership/lifetime role: writes encoded bytes into caller-provided output buffer; does not allocate or take ownership.

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: `memchr`, `memmove`.
- Data/type/global dependencies: COBS code-byte rules and caller-provided EOP byte.
- Artifact coverage: production wrapper caller has a generated artifact and is next in the active slice.

## Behavior

Encodes a byte range using COBS with optional EOP XOR:

1. Reserves the first output byte for the current block's code byte.
2. While input remains, limits the block to the remaining input or the maximum number of bytes before code `0xFF`.
3. Uses `memchr` to find the next zero in that block window.
4. Copies the non-zero chunk into `dst` with `memmove` so overlapping buffers are tolerated.
5. If `eop != 0`, XORs copied data bytes with `eop`.
6. Writes the block code byte XORed with `eop` when a zero terminates the block or the code limit is reached, then starts a new block.
7. Writes the final code byte and returns `dst - start`, excluding any separate packet terminator byte.

## Important Boundaries

- Preconditions and assumptions: `ptr` and `dst` must be valid for the caller-provided lengths; output buffer sizing is a caller responsibility.
- Error/null/empty behavior: no error channel and no NULL guard; zero-length input emits one code byte.
- Ownership and lifetime: mutates caller-provided `dst`; reads caller-provided `ptr`.
- Concurrency/global state: no global state.
- Portability/platform constraints: standard C memory routines and `uint8_t`/`uint32_t`.
- Performance shape: O(n), using `memchr` and `memmove` to avoid byte-by-byte scanning/copying for long runs.
- Security or parsing constraints: output excludes the EOP marker; callers append newline/EOP separately when using binary transport framing.

## Direct Call Sites

Exact production search `rg -n "\b_cobsEncode\b" -g '!source-research/**' -g '!docs/**'` found:

- Declaration: `n_lib.h:164`.
- Definition: `n_cobs.c:115`.
- Production wrapper call: `n_helpers.c:186`, from `NoteBinaryCodecEncode`.

Focused tests and HITL support code also call or fake this symbol, but they are outside production graph scope.

## Role In Callers

`NoteBinaryCodecEncode` supplies `NOTE_C_BINARY_EOP` (`'\n'`) as the EOP byte and handles NULL/buffer-size validation before delegating to this encoder.

## Tests Observed

Focused test search found `_cobsEncode` and wrapper tests, but production source was sufficient to finalize this node.

## Graph Links

- Graph node id: `function:n_cobs.c:115:_cobsEncode`
- Structured graph: `inventory/dependency-graph.json`
- Callee artifacts: none.
- Caller artifact candidates: `artifacts/functions/n_helpers_c__L169__NoteBinaryCodecEncode.md`.
- Upstream public behavior candidates: `NoteBinaryCodecEncode` and `NoteBinaryStoreTransmit`.

## Evidence Gaps

No unresolved implementation behavior. Caller artifacts remain preliminary, and caller-side buffer sizing is required because this helper does not validate output capacity.
