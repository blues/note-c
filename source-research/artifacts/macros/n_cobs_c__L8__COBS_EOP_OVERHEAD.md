# `COBS_EOP_OVERHEAD`

## Status

Finalized. Source-inspected COBS encoded-length overhead constant.

## Identity

- Kind: macro
- Definition: `n_cobs.c:8`
- Declaration/interface: file-local preprocessor constant
- Public/exported names: none
- Macro aliases/wrappers: none found
- Signature/API shape: `#define COBS_EOP_OVERHEAD 1`
- Visibility: private to `n_cobs.c`
- Ownership/lifetime role: encoded-size accounting constant, not runtime storage

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: used by `_cobsEncodedMaxLength`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edge

## Behavior

Adds one byte to `_cobsEncodedMaxLength` for the encoded packet end-of-packet marker. `_cobsEncodedLength` reports encoded payload length without this EOP overhead.

## Important Boundaries

- Preconditions and assumptions: applies to max encoded buffer sizing, not to decode output length.
- Error/null/empty behavior: not a runtime function.
- Ownership and lifetime: no ownership.
- Concurrency/global state: none.
- Portability/platform constraints: integer macro private to portable COBS code.
- Performance shape: constant arithmetic.
- Security or parsing constraints: undersizing this constant would under-allocate encoded packets that include EOP.

## Direct Call Sites

Exact source search found the definition and use in `_cobsEncodedMaxLength` at `n_cobs.c:266`. No other direct production uses were found.

## Role In Callers

`_cobsEncodedMaxLength` adds this constant after data length and COBS code-byte count to return a buffer size that includes the EOP byte.

## Tests Observed

Focused max-length and fit tests cover lengths such as 0, 254, 508, and round-trip fit boundaries in `_cobsEncodedMaxLength_test.cpp`. Gap: `_cobsGuaranteedFit` uses equivalent hardcoded two-byte total overhead rather than this macro.

## Graph Links

- Graph node id: `macro:n_cobs.c:8:COBS_EOP_OVERHEAD`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edge from `_cobsEncodedMaxLength`.

## Evidence Gaps

No unresolved source gaps for this node.
