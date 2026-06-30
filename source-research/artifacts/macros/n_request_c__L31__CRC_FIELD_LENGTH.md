# `CRC_FIELD_LENGTH`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_request.c:31`
- Declaration/interface: translation-unit-local CRC macro in `n_request.c` under `!NOTE_C_LOW_MEM`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `22`, matching `,"crc":"SSSS:CCCCCCCC"`.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: CRC helpers only when low-memory mode is disabled.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the number of bytes added by the CRC field and the number of bytes stripped/recognized from the response tail.

## Important Boundaries

- Disabled by `NOTE_C_LOW_MEM`.
- `_crcAdd` allocates `jsonLen + CRC_FIELD_LENGTH + 1`.
- `_crcError` uses the same length to reject too-short responses and locate the CRC tail.

## Direct Call Sites

Used by `_crcAdd` and `_crcError` in `n_request.c`.

## Role In Callers

Keeps CRC append and CRC validation offset math consistent.

## Tests Observed

Focused `_crcAdd_test.cpp` and `_crcError_test.cpp` cover valid, missing, and mismatched CRC behavior.

## Graph Links

- Graph node id: `macro:n_request.c:31:CRC_FIELD_LENGTH`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to `_crcAdd` and `_crcError`.

## Evidence Gaps

No unresolved source gaps for this node.
