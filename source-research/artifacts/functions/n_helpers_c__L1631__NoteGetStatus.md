# `NoteGetStatus`

## Status

Finalized. Source-inspected public immediate-refresh card-status wrapper, direct caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_helpers.c:1631`
- Declaration/interface: `note.h:1955`
- Public/exported names: `NoteGetStatus`
- Macro aliases/wrappers: none
- Signature/API shape: `bool NoteGetStatus(char *statusBuf, int statusBufLen, JTIME *bootTime, bool *retUSB, bool *retSignals)`
- Visibility: public note-c helper API
- Ownership/lifetime role: passes caller-provided outputs through to `NoteGetStatusST`

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:1649:NoteGetStatusST` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes static `statusTimer`
- Artifact coverage: source, declaration, direct caller search, and focused tests inspected

## Behavior

Sets static `statusTimer` to zero and returns `NoteGetStatusST(statusBuf, statusBufLen, bootTime, retUSB, retSignals)`. This forces the suppressed variant to attempt a refresh, but it does not clear the suppressed variant's function-local cached status values.

## Important Boundaries

- Preconditions and assumptions: output-buffer and pointer behavior is inherited from `NoteGetStatusST`.
- Error/null/empty behavior: refresh, cache, output-copy, and error behavior are delegated to `NoteGetStatusST`; this wrapper only resets the timer.
- Ownership and lifetime: no allocation in this wrapper.
- Concurrency/global state: mutates static `statusTimer` without local locking.
- Portability/platform constraints: transport and timing behavior remain behind the suppressed variant.
- Performance shape: one delegated refresh attempt or cache operation.
- Security or parsing constraints: no parsing in this wrapper.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This is the public non-suppressed card-status helper API.

## Tests Observed

`test/src/NoteGetStatus_test.cpp` covers no response and successful responses, including connected, disconnected, and connected-with-zero-signals cases.

## Test Coverage Notes

Focused tests do not directly assert `statusTimer` reset, suppressed cache-hit behavior, stale-cache behavior after a failed refresh, error responses carrying `"err"`, or the exact `"card.status"` request string.

## Graph Links

- Graph node id: `function:n_helpers.c:1631:NoteGetStatus`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
