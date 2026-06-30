# `CRC_FIELD_NAME_TEST`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_request.c:33`
- Declaration/interface: translation-unit-local CRC macro in `n_request.c` under `!NOTE_C_LOW_MEM`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal string `"\"crc\":\""`.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: CRC response validation helper.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Provides the exact CRC field prefix `_crcError` expects at the response tail.

## Important Boundaries

- Disabled by `NOTE_C_LOW_MEM`.
- Compared with `memcmp` using `sizeof(CRC_FIELD_NAME_TEST) - 1`.
- Only tail-position CRC fields count as protocol CRC metadata.

## Direct Call Sites

Used by `_crcError` for CRC tail recognition and payload pointer arithmetic.

## Role In Callers

Separates protocol CRC metadata from arbitrary JSON content.

## Tests Observed

Focused `_crcError_test.cpp` covers CRC present, absent, and mismatched cases.

## Graph Links

- Graph node id: `macro:n_request.c:33:CRC_FIELD_NAME_TEST`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `_crcError`.

## Evidence Gaps

No unresolved source gaps for this node.
