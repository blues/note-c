# `NoteGetFnTransaction`

## Status

Finalized. Source-inspected public transaction hook getter.

## Identity

- Kind: function
- Definition: `n_hooks.c:725`
- Declaration/interface: `note.h:575`
- Public/exported names: `NoteGetFnTransaction`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteGetFnTransaction(txnStartFn *startFn, txnStopFn *stopFn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: copies current transaction start/stop hook pointers into caller-provided storage

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads `hookTransactionStart` and `hookTransactionStop`
- Artifact coverage: source-inspected artifact

## Behavior

Locks Notecard access, writes `hookTransactionStart` to `*startFn` if requested, writes `hookTransactionStop` to `*stopFn` if requested, and unlocks. Null output pointers are skipped independently.

## Important Boundaries

- Preconditions and assumptions: non-null output pointers must point to writable storage for their matching function-pointer types.
- Error/null/empty behavior: either output pointer may be null and is skipped; all-null outputs still lock and unlock.
- Ownership and lifetime: copied callbacks remain owned by the caller/platform that installed them.
- Concurrency/global state: serializes reads of transaction hook globals with `_LockNote`/`_UnlockNote`.
- Portability/platform constraints: transaction hooks wrap portable request orchestration and do not own transport behavior.
- Performance shape: one lock, up to two pointer copies, one unlock.
- Security or parsing constraints: no validation that copied callbacks remain valid after return.

## Direct Call Sites

Source/header exact search found only the public declaration in `note.h:575`, the definition in `n_hooks.c:725`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public hook-introspection API for downstream callers that need transaction lifecycle hook state.

## Tests Observed

Focused tests in `test/src/NoteGetFnTransaction_test.cpp` cover null outputs, both outputs, single outputs, and lock/unlock counts. Transaction hook tests cover dispatch wrapper behavior. Gap: default null hook state after clearing is not directly asserted by this getter test.

## Graph Links

- Graph node id: `function:n_hooks.c:725:NoteGetFnTransaction`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockNote` and `_UnlockNote`.

## Evidence Gaps

No unresolved source gaps for this node.
