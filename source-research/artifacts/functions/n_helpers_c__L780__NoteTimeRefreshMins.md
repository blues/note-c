# `NoteTimeRefreshMins`

## Status

Finalized. Source-inspected public time-refresh interval setter.

## Identity

- Kind: function
- Definition: `n_helpers.c:780`
- Declaration/interface: `note.h:1720`
- Public/exported names: `NoteTimeRefreshMins`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteTimeRefreshMins(uint32_t mins)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: updates module-local time refresh configuration

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes static `refreshTimerSecs`, which defaults to 300 seconds (five minutes) and is read by `NoteTimeST`
- Artifact coverage: source-inspected artifact

## Behavior

Stores `mins * 60` in the module-local `refreshTimerSecs` value. `NoteTimeST` later checks `refreshTimerSecs != 0` before using `_timerExpiredSecs(&timeRefreshTimer, refreshTimerSecs)` to invalidate the cached time by setting `timeTimer = 0`.

## Important Boundaries

- Preconditions and assumptions: accepts a primitive `uint32_t` minute count; no pointer or ownership boundary.
- Error/null/empty behavior: no return value and no error path. Passing zero stores zero, which disables automatic refresh in `NoteTimeST`.
- Ownership and lifetime: writes only module-local static state.
- Concurrency/global state: no locking around `refreshTimerSecs`; concurrent callers or `NoteTimeST` readers can observe unsynchronized updates.
- Portability/platform constraints: `mins * 60` is computed in `uint32_t` arithmetic and may wrap.
- Performance shape: constant-time scalar assignment.
- Security or parsing constraints: no input validation or overflow check.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1720` and the definition in `n_helpers.c:780`. No production repository caller, test caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public configuration helper for downstream callers that want to adjust or disable cached Notecard time refresh.

## Tests Observed

Focused `test/src/NoteTime_test.cpp` covers a five-minute `NoteTimeRefreshMins` period driving a second `card.time` fetch via `NoteTimeST` after the interval elapses, while an in-window `NoteTimeST` call does not re-fetch. Gaps remain for zero disabling refresh, minutes-to-seconds conversion edge cases, and overflow.

## Graph Links

- Graph node id: `function:n_helpers.c:780:NoteTimeRefreshMins`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
