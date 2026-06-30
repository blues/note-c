# `NoteSetFnDefault`

## Status

Finalized. Source-inspected public default memory/timing hook setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:248`
- Declaration/interface: `note.h:645`
- Public/exported names: `NoteSetFnDefault`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFnDefault(mallocFn mallocfn, freeFn freefn, delayMsFn delayfn, getMsFn millisfn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: conditionally stores caller-provided callback pointers as default SDK hook globals

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes `hookMalloc`, `hookFree`, `hookDelayMs`, and `hookGetMs` only when each current slot is null
- Artifact coverage: source-inspected artifact

## Behavior

Locks Notecard access, checks each memory/timing hook slot independently, stores the supplied callback only when that slot is currently null, then unlocks. A null argument can become the stored default only when the current slot is already null; it does not clear an already-set hook.

## Important Boundaries

- Preconditions and assumptions: callback pointers are accepted as opaque values and are not validated.
- Error/null/empty behavior: no return value or error path; null callbacks are accepted.
- Ownership and lifetime: callback ownership stays with the caller/platform; note-c stores the raw function pointers.
- Concurrency/global state: updates global hook slots under `_LockNote`/`_UnlockNote`.
- Portability/platform constraints: this is the portable hook boundary for allocation and timing callbacks.
- Performance shape: one lock, up to four conditional pointer assignments, one unlock.
- Security or parsing constraints: no validation that callbacks remain valid after return.

## Direct Call Sites

Source/header exact search found only the public declaration in `note.h:645`, the definition in `n_hooks.c:248`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public API for downstream adapter code that wants to provide fallback allocation/free/delay/millis hooks without overriding hooks already installed.

## Tests Observed

Focused tests in `test/src/NoteSetFnDefault_test.cpp` verify storage and lock behavior. Consumer tests for `NoteMalloc`, `NoteFree`, `NoteDelayMs`, and `NoteGetMs` exercise the resulting hook values. Gaps include mixed set/unset permutations and concurrency.

## Graph Links

- Graph node id: `function:n_hooks.c:248:NoteSetFnDefault`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockNote` and `_UnlockNote`.

## Evidence Gaps

No unresolved source gaps for this node.
