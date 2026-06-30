# `COBS_MAX_PACKET_SIZE`

## Status

Finalized. Source-inspected COBS maximum packet-block size constant.

## Identity

- Kind: macro
- Definition: `n_cobs.c:9`
- Declaration/interface: file-local preprocessor constant
- Public/exported names: none
- Macro aliases/wrappers: none found
- Signature/API shape: `#define COBS_MAX_PACKET_SIZE 254`
- Visibility: private to `n_cobs.c`
- Ownership/lifetime role: encoded-size code-byte grouping constant

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: used by `_cobsEncodedMaxLength`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edge

## Behavior

Defines the 254-byte COBS data block size used by `_cobsEncodedMaxLength` to compute code bytes as `(length / COBS_MAX_PACKET_SIZE) + 1`. Exact multiples of 254 still need an additional final code byte.

## Important Boundaries

- Preconditions and assumptions: applies to max-length accounting; other COBS internals use equivalent literal values such as `0xFF`, `255`, and `2`.
- Error/null/empty behavior: not a runtime function.
- Ownership and lifetime: no ownership.
- Concurrency/global state: none.
- Portability/platform constraints: integer macro private to portable COBS code.
- Performance shape: constant division in size computation.
- Security or parsing constraints: wrong value would under/overestimate encoded buffer requirements.

## Direct Call Sites

Exact source search found the definition and use in `_cobsEncodedMaxLength` at `n_cobs.c:265`. No other direct production uses of this macro were found.

## Role In Callers

`_cobsEncodedMaxLength` uses this constant to account for the initial code byte plus one code byte for every 254 source bytes.

## Tests Observed

Focused max-length tests cover 253, 254, 255, 508, 65536, and fit boundaries; encode/decode tests cover the 254-byte block break via `0xFF` and a second code byte. Gap: encoder and fit internals use hardcoded equivalent literals rather than this macro.

## Graph Links

- Graph node id: `macro:n_cobs.c:9:COBS_MAX_PACKET_SIZE`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edge from `_cobsEncodedMaxLength`.

## Evidence Gaps

No unresolved source gaps for this node.
