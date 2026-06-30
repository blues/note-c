# `_setTime`

## Status

Finalized. Source-inspected internal time-base update helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:791`
- Declaration/interface: file-local prototype at `n_helpers.c:77`
- Signature/API shape: `void _setTime(JTIME seconds)`
- Visibility: internal to `n_helpers.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none
- Macro aliases/wrappers: none
- Ownership/lifetime role: updates global time-base cache

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:243:_GetMs` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: mutates `timeBaseSec` and `timeBaseSetAtMs`
- Artifact coverage: generated artifact exists

## Behavior

Stores `seconds` in `timeBaseSec` and records the current millisecond tick from `_GetMs()` in `timeBaseSetAtMs`.

## Important Boundaries

- Preconditions and assumptions: caller has already decided that `seconds` is a valid nonzero time when that distinction matters.
- Error/null/empty behavior: no validation; passing `0` clears the base seconds while still recording the current millisecond tick.
- Ownership and lifetime: no allocation.
- Concurrency/global state: writes global time cache state without locking.
- Portability/platform constraints: millisecond source remains behind the `_GetMs` hook alias.
- Performance shape: O(1).
- Security or parsing constraints: none.

## Direct Call Sites

Exact source search found direct callers:

- `n_helpers.c:903` `NoteTimeST` stores nonzero `card.time` results.
- `n_helpers.c:1856` `NotePayloadRetrieveAfterSleep` stores nonzero `card.attn` wake-time results.

## Role In Callers

This helper is the common cache update point when time is learned from the Notecard through normal time polling or sleep payload retrieval.

## Tests Observed

No direct `_setTime` test calls were found, though `test/include/test_static.h:28` exposes it. `NoteTime_test.cpp` and `NotePayloadRetrieveAfterSleep_test.cpp` cover it indirectly through public callers.

## Graph Links

- Graph node id: `function:n_helpers.c:791:_setTime`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_GetMs`.

## Evidence Gaps

No unresolved source gaps for this node.
