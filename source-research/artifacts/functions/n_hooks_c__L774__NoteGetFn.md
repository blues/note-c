# `NoteGetFn`

## Status

Finalized. Source-inspected public hook accessor for memory and timing callbacks.

## Identity

- Kind: function
- Definition: `n_hooks.c:774`
- Declaration/interface: `note.h:672`
- Public/exported names: `NoteGetFn`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteGetFn(mallocFn *mallocHook, freeFn *freeHook, delayMsFn *delayMsHook, getMsFn *getMsHook)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: copies current hook function-pointer values into caller-provided output slots

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads static hook globals `hookMalloc`, `hookFree`, `hookDelayMs`, and `hookGetMs`
- Artifact coverage: source-inspected artifact

## Behavior

Locks Notecard access with `_LockNote`, conditionally writes each requested output pointer with the corresponding current hook global, then unlocks with `_UnlockNote`. Null output pointers are ignored individually. The function has no return value and performs no allocation or validation beyond null checks on the output slots.

## Important Boundaries

- Preconditions and assumptions: any non-null output pointer must point to writable storage for the matching function-pointer type.
- Error/null/empty behavior: any output slot may be null; null slots are skipped without error. If all slots are null, the function still locks and unlocks.
- Ownership and lifetime: the copied function pointers remain owned/configured by the SDK hook layer; no callback ownership changes.
- Concurrency/global state: serializes reads of the four hook globals using the Notecard mutex hook path.
- Portability/platform constraints: depends on platform-supplied mutex hooks only through `_LockNote` and `_UnlockNote`.
- Performance shape: one lock, up to four pointer copies, one unlock.
- Security or parsing constraints: no parsing; no validation that stored hook pointers remain valid after return.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:672`, the definition in `n_hooks.c:774`, and focused tests in `test/src/NoteGetFn_test.cpp`. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public hook-introspection API for downstream callers that need to inspect currently configured allocation and timing callbacks.

## Tests Observed

Focused tests in `test/src/NoteGetFn_test.cpp` directly set the hook globals and verify that all-null outputs do not crash, each individual output pointer receives the configured hook, all outputs can be returned together, and `_LockNote`/`_UnlockNote` call counts remain balanced. Gaps include concurrent mutation by another thread and behavior when mutex hooks themselves fail, which cannot be represented by the void hook signatures.

## Graph Links

- Graph node id: `function:n_hooks.c:774:NoteGetFn`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockNote` and `_UnlockNote`.

## Evidence Gaps

No unresolved source gaps for this node.
