# `NoteGetFnSerial`

## Status

Finalized. Source-inspected public serial hook getter.

## Identity

- Kind: function
- Definition: `n_hooks.c:793`
- Declaration/interface: `note.h:715`
- Public/exported names: `NoteGetFnSerial`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteGetFnSerial(serialResetFn *resetFn, serialTransmitFn *transmitFn, serialAvailableFn *availFn, serialReceiveFn *receiveFn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: copies current serial hook pointers into caller-provided storage

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads `hookSerialReset`, `hookSerialTransmit`, `hookSerialAvailable`, and `hookSerialReceive`
- Artifact coverage: source-inspected artifact

## Behavior

Locks Notecard access, conditionally writes each requested output pointer with the corresponding current serial hook, and unlocks. Null output pointers are skipped independently.

## Important Boundaries

- Preconditions and assumptions: non-null output pointers must point to writable storage for matching function-pointer types.
- Error/null/empty behavior: any output pointer may be null; all-null calls still lock and unlock.
- Ownership and lifetime: copied callbacks remain owned by the caller/platform that installed them.
- Concurrency/global state: serializes reads of serial hook globals with `_LockNote`/`_UnlockNote`.
- Portability/platform constraints: this is the introspection boundary for platform serial hooks.
- Performance shape: one lock, up to four pointer copies, one unlock.
- Security or parsing constraints: no validation that copied callbacks remain valid after return.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:715`, the definition in `n_hooks.c:793`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public hook-introspection API for downstream callers that need current serial callback state.

## Tests Observed

Focused tests in `test/src/NoteGetFnSerial_test.cpp` cover all-null, all outputs, each single output, and lock/unlock counts. Gap: no concurrent mutation coverage.

## Graph Links

- Graph node id: `function:n_hooks.c:793:NoteGetFnSerial`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockNote` and `_UnlockNote`.

## Evidence Gaps

No unresolved source gaps for this node.
