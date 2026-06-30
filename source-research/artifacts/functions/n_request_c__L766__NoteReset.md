# `NoteReset`

## Status

Finalized. Source-inspected public reset wrapper, shared reset-required state, finalized reset/lock callees, direct caller, and focused tests.

## Identity

- Kind: function
- Definition: `n_request.c:766`
- Declaration/interface: `note.h:298`
- Public/exported names: `NoteReset`
- Macro aliases/wrappers: none
- Signature/API shape: `bool NoteReset(void)`
- Visibility: public SDK API declared in `note.h`
- Ownership/lifetime role: no allocation or ownership transfer; updates reset-required state

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:237:_Reset` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: writes `NOTE_C_STATIC bool resetRequired` in `n_request.c`; `_Reset` dispatches through the active-interface reset hook selected elsewhere
- Artifact coverage: source, declaration, direct caller, shared reset flag, finalized callees, and focused tests inspected

## Behavior

Acquires the Notecard lock with `_LockNote`, calls `_Reset`, stores the inverse result in the shared `resetRequired` flag, releases the lock with `_UnlockNote`, and returns `!resetRequired`. A successful `_Reset()` call clears `resetRequired` and returns true from `NoteReset`; a failed `_Reset()` call leaves `resetRequired` true and returns false.

## Important Boundaries

- Preconditions and assumptions: no parameters; active interface/reset hook selection is managed elsewhere.
- Error/null/empty behavior: no error string or diagnostic is returned. Through finalized `_Reset`, a missing active reset hook returns true and therefore clears `resetRequired`.
- Ownership and lifetime: no allocation and no ownership transfer.
- Concurrency/global state: wraps the reset call and `resetRequired` update in the optional Notecard mutex hook. If mutex hooks are unset, lock/unlock are no-ops.
- Portability/platform constraints: platform reset behavior stays behind `_noteHardReset` and active-interface reset hook dispatch.
- Performance shape: O(reset hook cost).
- Security or parsing constraints: none.

## Direct Call Sites

Exact source search found one direct production caller:

- `n_helpers.c:1923` `NoteFactoryReset` waits 5 seconds after `card.restore`, logs restoration, then loops calling `NoteReset()` and delaying 5 seconds until reset succeeds.

The public declaration is `note.h:298`.

## Role In Callers

`NoteFactoryReset` treats `NoteReset` as the post-restore reinitialization gate. It does not return success until the reset wrapper reports success.

## Tests Observed

`test/src/NoteReset_test.cpp` fakes `_noteHardReset` and verifies it is invoked and that false/true reset-hook results propagate through `NoteReset`. `test/src/NoteFactoryReset_test.cpp` fakes `NoteReset` and verifies `NoteFactoryReset` retries after one reset failure. `_noteHardReset_test.cpp` verifies the callee behavior that a null active reset hook returns true and a non-null hook result is forwarded.

## Test Coverage Notes

Focused tests do not directly assert the `resetRequired` flag transition or lock/unlock calls in `NoteReset`.

## Graph Links

- Graph node id: `function:n_request.c:766:NoteReset`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
