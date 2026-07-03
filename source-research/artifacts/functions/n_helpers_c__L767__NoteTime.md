# `NoteTime`

## Status

Finalized. Source-inspected public unsuppressed time wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:767`
- Declaration/interface: `note.h:1708`
- Signature/API shape: `JTIME NoteTime(void)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NoteTime`
- Macro aliases/wrappers: none
- Ownership/lifetime role: resets suppression timer state and delegates to `NoteTimeST`

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:871:NoteTimeST` finalized in this slice
- External/system dependencies: none directly
- Data/type/global dependencies: mutates global `timeTimer`
- Artifact coverage: generated artifact exists

## Behavior

Sets `timeTimer = 0` to suppress no fetch attempt, then returns `NoteTimeST()`.

## Important Boundaries

- Preconditions and assumptions: none.
- Error/null/empty behavior: inherits `NoteTimeST` behavior, including boot-seconds fallback when no real time is cached.
- Ownership and lifetime: no allocation.
- Concurrency/global state: writes global `timeTimer` without locking and delegates to global time-cache logic.
- Portability/platform constraints: inherits millisecond hook behavior from `NoteTimeST`.
- Performance shape: O(1) plus any refresh transaction performed by `NoteTimeST`.
- Security or parsing constraints: none.

## Direct Call Sites

No in-repository production call sites were found beyond the definition and public declaration in `note.h:1708`.

## Role In Callers

This is the public force-refresh wrapper above session-time retrieval.

## Tests Observed

Focused `test/src/NoteTime_test.cpp` covers null/error responses, valid card time, and millisecond rollover through this wrapper.

## Graph Links

- Graph node id: `function:n_helpers.c:767:NoteTime`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `NoteTimeST`.

## Evidence Gaps

No unresolved source gaps for this node.
