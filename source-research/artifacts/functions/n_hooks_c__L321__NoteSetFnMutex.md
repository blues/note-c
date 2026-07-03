# `NoteSetFnMutex`

## Status

Finalized. Source-inspected public I2C and Notecard mutex hook setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:321`
- Declaration/interface: `note.h:584`
- Public/exported names: `NoteSetFnMutex`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFnMutex(mutexFn lockI2Cfn, mutexFn unlockI2Cfn, mutexFn lockNotefn, mutexFn unlockNotefn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: stores caller-provided I2C and Notecard lock/unlock callback pointers

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes `hookLockI2C`, `hookUnlockI2C`, `hookLockNote`, and `hookUnlockNote`
- Artifact coverage: source-inspected artifact

## Behavior

Directly assigns all four mutex hook globals from the supplied arguments. Unlike most hook setters, this function does not call `_LockNote` or `_UnlockNote`.

## Important Boundaries

- Preconditions and assumptions: callback pointers are accepted as opaque values.
- Error/null/empty behavior: no return value or error path; null clears the corresponding hook. Runtime lock/unlock wrappers no-op when hooks are null.
- Ownership and lifetime: callback ownership stays with the caller/platform; note-c stores raw function pointers.
- Concurrency/global state: unsynchronized writes to global mutex hook pointers.
- Portability/platform constraints: this is the platform boundary for I2C and Notecard mutex callbacks.
- Performance shape: four pointer assignments.
- Security or parsing constraints: no validation that callbacks remain valid after return.

## Direct Call Sites

Source/header exact search found only the public declaration in `note.h:584`, the definition in `n_hooks.c:321`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public API for downstream adapter code that wants to install or clear both I2C and Notecard mutex hooks.

## Tests Observed

Focused tests in `test/src/NoteSetFnMutex_test.cpp` and `test/src/NoteGetFnMutex_test.cpp` cover setting/getting and clearing behavior. Gaps include partial null permutations and concurrency.

## Graph Links

- Graph node id: `function:n_hooks.c:321:NoteSetFnMutex`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
