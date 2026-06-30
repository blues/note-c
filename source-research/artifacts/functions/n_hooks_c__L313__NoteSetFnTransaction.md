# `NoteSetFnTransaction`

## Status

Finalized. Source-inspected public transaction hook setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:313`
- Declaration/interface: `note.h:565`
- Public/exported names: `NoteSetFnTransaction`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFnTransaction(txnStartFn startFn, txnStopFn stopFn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: stores transaction start/stop callback pointers

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes `hookTransactionStart` and `hookTransactionStop`
- Artifact coverage: source-inspected artifact

## Behavior

Locks Notecard access, assigns transaction start and stop hook globals from the supplied arguments, and unlocks. Null arguments clear the corresponding transaction hook; `_noteTransactionStart` defaults to success when unset, and `_noteTransactionStop` no-ops when unset.

## Important Boundaries

- Preconditions and assumptions: callback pointers are accepted as opaque values.
- Error/null/empty behavior: no return value or error path; null clears hooks.
- Ownership and lifetime: callback ownership stays with the caller/platform; note-c stores raw function pointers.
- Concurrency/global state: updates transaction hooks under `_LockNote`/`_UnlockNote`.
- Portability/platform constraints: transaction hooks wrap portable request orchestration without owning platform transport behavior.
- Performance shape: one lock, two pointer assignments, one unlock.
- Security or parsing constraints: no validation that callbacks remain valid after return.

## Direct Call Sites

Source/header exact search found only the public declaration in `note.h:565`, the definition in `n_hooks.c:313`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public API for downstream adapter code that wants transaction lifecycle callbacks.

## Tests Observed

Focused tests in `test/src/NoteSetFnTransaction_test.cpp`, `test/src/NoteGetFnTransaction_test.cpp`, and transaction hook tests cover setting/getting and dispatch behavior. Gaps include false-return propagation from a registered start hook in the focused dispatcher coverage.

## Graph Links

- Graph node id: `function:n_hooks.c:313:NoteSetFnTransaction`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockNote` and `_UnlockNote`.

## Evidence Gaps

No unresolved source gaps for this node.
