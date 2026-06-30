# `ERR_FIELD_NAME_TEST`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_request.c:34`
- Declaration/interface: translation-unit-local CRC macro in `n_request.c` under `!NOTE_C_LOW_MEM`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal string `"\"err\":\""`.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: CRC response validation helper.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Lets `_crcError` bypass CRC checks when the response already contains a Notecard error field.

## Important Boundaries

- Disabled by `NOTE_C_LOW_MEM`.
- Uses substring search, so it is an early protocol-error bypass before CRC length/tail checks.

## Direct Call Sites

Used by `_crcError` before the minimum CRC-length and tail-matching logic.

## Role In Callers

Prevents Notecard error responses from being retried as CRC failures.

## Tests Observed

Focused `_crcError_test.cpp` covers error response handling with CRC support state.

## Graph Links

- Graph node id: `macro:n_request.c:34:ERR_FIELD_NAME_TEST`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `_crcError`.

## Evidence Gaps

No unresolved source gaps for this node.
