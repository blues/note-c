# `NoteSetFnNoteMutex`

## Status

Finalized. Source-inspected public Notecard mutex hook setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:335`
- Declaration/interface: `note.h:622`
- Public/exported names: `NoteSetFnNoteMutex`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFnNoteMutex(mutexFn lockFn, mutexFn unlockFn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: stores caller-provided Notecard lock/unlock callback pointers

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes `hookLockNote` and `hookUnlockNote`
- Artifact coverage: source-inspected artifact

## Behavior

Directly assigns the Notecard mutex lock and unlock hook globals from the supplied arguments. This function does not call `_LockNote` or `_UnlockNote`.

## Important Boundaries

- Preconditions and assumptions: callback pointers are accepted as opaque values.
- Error/null/empty behavior: no return value or error path; null clears the corresponding hook. Runtime Notecard lock/unlock wrappers no-op when hooks are null.
- Ownership and lifetime: callback ownership stays with the caller/platform; note-c stores raw function pointers.
- Concurrency/global state: unsynchronized writes to global Notecard mutex hook pointers.
- Portability/platform constraints: this is the platform boundary for Notecard mutex callbacks.
- Performance shape: two pointer assignments.
- Security or parsing constraints: no validation that callbacks remain valid after return.

## Direct Call Sites

Source/header exact search found only the public declaration in `note.h:622`, the definition in `n_hooks.c:335`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public API for downstream adapter code that wants to install or clear the Notecard mutex hooks.

## Tests Observed

Focused tests in `test/src/NoteSetFnNoteMutex_test.cpp` and `test/src/NoteGetFnNoteMutex_test.cpp` cover setting/getting and clearing behavior. Gaps include one-sided null permutations and concurrency.

## Graph Links

- Graph node id: `function:n_hooks.c:335:NoteSetFnNoteMutex`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
