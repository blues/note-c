# `NoteBinaryCodecMaxDecodedLength`

## Status

Finalized. Source-inspected public binary codec decoded-length wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:218`
- Declaration/interface: `note.h:1628`
- Public/exported names: `NoteBinaryCodecMaxDecodedLength`
- Macro aliases/wrappers: none found
- Signature/API shape: `uint32_t NoteBinaryCodecMaxDecodedLength(uint32_t bufferSize)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: pure sizing wrapper; owns no memory and touches no caller buffers

## Dependency Status

- Project-local dependencies:
  - `function:n_cobs.c:305:_cobsGuaranteedFit` finalized
- External/system dependencies: none
- Data/type/global dependencies: none beyond integer arithmetic in the finalized COBS helper
- Artifact coverage: source-inspected artifact

## Behavior

Returns `_cobsGuaranteedFit(bufferSize)` without modifying the argument or applying additional checks.

## Important Boundaries

- Preconditions and assumptions: accepts any `uint32_t` buffer size; boundary handling is delegated to `_cobsGuaranteedFit`.
- Error/null/empty behavior: no pointer inputs or local failure path. For small buffer sizes, finalized `_cobsGuaranteedFit` returns zero.
- Ownership and lifetime: no ownership or lifetime effects.
- Concurrency/global state: no global state.
- Portability/platform constraints: portable integer arithmetic through the finalized COBS helper.
- Performance shape: O(1) wrapper.
- Security or parsing constraints: no parsing; overflow/edge behavior is inherited from finalized `_cobsGuaranteedFit`.

## Direct Call Sites

Exact source search found the public declaration in `note.h:1628`, the definition in `n_helpers.c:218`, a source comment reference, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that need the maximum unencoded payload length guaranteed to fit in an encoded buffer.

## Tests Observed

Focused tests in `test/src/NoteBinaryCodecMaxDecodedLength_test.cpp` fake `_cobsGuaranteedFit` and verify the argument and return value are passed through unchanged. They do not exercise real COBS arithmetic edge cases through this wrapper.

## Graph Links

- Graph node id: `function:n_helpers.c:218:NoteBinaryCodecMaxDecodedLength`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_cobsGuaranteedFit`.

## Evidence Gaps

No unresolved source gaps for this node.
