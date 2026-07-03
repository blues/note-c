# `NoteBinaryCodecMaxEncodedLength`

## Status

Finalized. Source-inspected public binary codec encoded-length wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:233`
- Declaration/interface: `note.h:1636`
- Public/exported names: `NoteBinaryCodecMaxEncodedLength`
- Macro aliases/wrappers: none found
- Signature/API shape: `uint32_t NoteBinaryCodecMaxEncodedLength(uint32_t unencodedLength)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: pure sizing wrapper; owns no memory and touches no caller buffers

## Dependency Status

- Project-local dependencies:
  - `function:n_cobs.c:263:_cobsEncodedMaxLength` finalized
- External/system dependencies: none
- Data/type/global dependencies: none beyond integer arithmetic in the finalized COBS helper
- Artifact coverage: source-inspected artifact

## Behavior

Returns `_cobsEncodedMaxLength(unencodedLength)` without modifying the argument or applying additional checks.

## Important Boundaries

- Preconditions and assumptions: accepts any `uint32_t` unencoded length; boundary and overflow behavior are delegated to `_cobsEncodedMaxLength`.
- Error/null/empty behavior: no pointer inputs or local failure path.
- Ownership and lifetime: no ownership or lifetime effects.
- Concurrency/global state: no global state.
- Portability/platform constraints: portable integer arithmetic through the finalized COBS helper.
- Performance shape: O(1) wrapper.
- Security or parsing constraints: no parsing; sizing behavior is inherited from finalized `_cobsEncodedMaxLength`.

## Direct Call Sites

Exact source search found the public declaration in `note.h:1636`, the definition in `n_helpers.c:233`, source comment references, focused unit tests, and HITL test/support uses under `test/`. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that need the maximum encoded buffer size for an unencoded payload length.

## Tests Observed

Focused tests in `test/src/NoteBinaryCodecMaxEncodedLength_test.cpp` fake `_cobsEncodedMaxLength` and verify the argument and return value are passed through unchanged. HITL test/support code under `test/hitl/card.binary` calls this helper for buffer sizing. The focused unit test does not exercise real COBS arithmetic edge cases through this wrapper.

## Graph Links

- Graph node id: `function:n_helpers.c:233:NoteBinaryCodecMaxEncodedLength`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_cobsEncodedMaxLength`.

## Evidence Gaps

No unresolved source gaps for this node.
