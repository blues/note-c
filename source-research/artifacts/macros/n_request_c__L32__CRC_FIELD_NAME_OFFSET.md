# `CRC_FIELD_NAME_OFFSET`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_request.c:32`
- Declaration/interface: translation-unit-local CRC macro in `n_request.c` under `!NOTE_C_LOW_MEM`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `1`.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: CRC response validation helper.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Offsets CRC field matching past the separator character at the computed response-tail location.

## Important Boundaries

- Disabled by `NOTE_C_LOW_MEM`.
- Meaningful only together with `CRC_FIELD_LENGTH` and `CRC_FIELD_NAME_TEST`.

## Direct Call Sites

Used by `_crcError` in the `memcmp` test and pointer to sequence/CRC payload.

## Role In Callers

Keeps CRC field-name matching aligned after the response object separator.

## Tests Observed

Focused `_crcError_test.cpp` covers valid and invalid CRC field recognition.

## Graph Links

- Graph node id: `macro:n_request.c:32:CRC_FIELD_NAME_OFFSET`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `_crcError`.

## Evidence Gaps

No unresolved source gaps for this node.
