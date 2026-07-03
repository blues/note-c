# `NoteSetFn`

## Status

Finalized. Source-inspected public memory/timing hook setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:266`
- Declaration/interface: `note.h:659`
- Public/exported names: `NoteSetFn`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFn(mallocFn mallocHook, freeFn freeHook, delayMsFn delayMsHook, getMsFn getMsHook)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: unconditionally stores caller-provided callback pointers as SDK hook globals

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes `hookMalloc`, `hookFree`, `hookDelayMs`, and `hookGetMs`
- Artifact coverage: source-inspected artifact

## Behavior

Locks Notecard access, assigns all four memory/timing hook globals from the supplied arguments, and unlocks. Null arguments clear the corresponding hook slots.

## Important Boundaries

- Preconditions and assumptions: callback pointers are accepted as opaque values and are not validated.
- Error/null/empty behavior: no return value or error path; null callbacks are accepted and clear hooks.
- Ownership and lifetime: callback ownership stays with the caller/platform; note-c stores raw function pointers.
- Concurrency/global state: updates global hook slots under `_LockNote`/`_UnlockNote`.
- Portability/platform constraints: this is the portable hook boundary for allocation and timing callbacks.
- Performance shape: one lock, four pointer assignments, one unlock.
- Security or parsing constraints: no validation that callbacks remain valid after return.

## Direct Call Sites

Source/header exact search found only the public declaration in `note.h:659`, the definition in `n_hooks.c:266`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public API for downstream adapter code that wants to install or clear allocation/free/delay/millis hooks.

## Tests Observed

Focused tests in `test/src/NoteSetFn_test.cpp` verify setting hooks, clearing hooks with nulls, and lock/unlock pairing. Gaps include partial argument permutations and concurrency.

## Graph Links

- Graph node id: `function:n_hooks.c:266:NoteSetFn`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockNote` and `_UnlockNote`.

## Evidence Gaps

No unresolved source gaps for this node.
