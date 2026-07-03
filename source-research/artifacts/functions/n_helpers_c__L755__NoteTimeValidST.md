# `NoteTimeValidST`

## Status

Finalized. Source-inspected public session-time validity wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:755`
- Declaration/interface: `note.h:1702`
- Signature/API shape: `bool NoteTimeValidST(void)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NoteTimeValidST`
- Macro aliases/wrappers: none
- Ownership/lifetime role: refreshes cached time and reports whether a real base time is known

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:871:NoteTimeST` finalized in this slice
- External/system dependencies: none directly
- Data/type/global dependencies: reads global `timeBaseSec`
- Artifact coverage: generated artifact exists

## Behavior

Calls `NoteTimeST()` to give the cache a chance to refresh, then returns whether `timeBaseSec != 0`.

## Important Boundaries

- Preconditions and assumptions: `timeBaseSec == 0` represents unknown real time.
- Error/null/empty behavior: returns false when refresh fails or no real time is cached.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads global time cache without locking after `NoteTimeST` may mutate it.
- Portability/platform constraints: inherits hook/request behavior from `NoteTimeST`.
- Performance shape: O(1) plus any refresh transaction performed by `NoteTimeST`.
- Security or parsing constraints: none.

## Direct Call Sites

Exact source search found direct production callers:

- `n_helpers.c:746` `NoteTimeValid`.
- `n_helpers.c:1048` `NoteLocalTimeST`.

## Role In Callers

This is the session-preserving validity wrapper; `NoteTimeValid` resets suppression before calling it.

## Tests Observed

No direct focused tests were found. `NoteTime_test.cpp` exercises the underlying refresh behavior through `NoteTime`; `NoteLocalTimeST_test.cpp` fakes this wrapper.

## Graph Links

- Graph node id: `function:n_helpers.c:755:NoteTimeValidST`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `NoteTimeST`.

## Evidence Gaps

No unresolved source gaps for this node.
