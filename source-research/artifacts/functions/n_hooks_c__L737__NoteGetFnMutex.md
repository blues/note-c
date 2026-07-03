# `NoteGetFnMutex`

## Status

Finalized. Source-inspected public I2C and Notecard mutex hook getter.

## Identity

- Kind: function
- Definition: `n_hooks.c:737`
- Declaration/interface: `note.h:597`
- Public/exported names: `NoteGetFnMutex`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteGetFnMutex(mutexFn *lockI2Cfn, mutexFn *unlockI2Cfn, mutexFn *lockNotefn, mutexFn *unlockNotefn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: copies current I2C and Notecard mutex hook pointers into caller-provided storage

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `hookLockI2C`, `hookUnlockI2C`, `hookLockNote`, and `hookUnlockNote`
- Artifact coverage: source-inspected artifact

## Behavior

Conditionally writes each requested output pointer with the matching current mutex hook global. Null output pointers are skipped independently. The getter does not lock.

## Important Boundaries

- Preconditions and assumptions: non-null output pointers must point to writable storage for the matching function-pointer type.
- Error/null/empty behavior: any output pointer may be null and is skipped; no return value or error path.
- Ownership and lifetime: copied callbacks remain owned by the caller/platform that installed them.
- Concurrency/global state: unsynchronized reads of global mutex hook pointers.
- Portability/platform constraints: this is the introspection boundary for platform mutex hooks.
- Performance shape: up to four null checks and pointer copies.
- Security or parsing constraints: no validation that copied callbacks remain valid after return.

## Direct Call Sites

Source/header exact search found only the public declaration in `note.h:597`, the definition in `n_hooks.c:737`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public hook-introspection API for downstream callers that need both I2C and Notecard mutex callback state.

## Tests Observed

Focused tests in `test/src/NoteGetFnMutex_test.cpp` cover all outputs, all null outputs, and each single output. Setter/dispatch tests cover clearing behavior. Gap: getter-after-clear/default null state is not directly asserted by this getter test.

## Graph Links

- Graph node id: `function:n_hooks.c:737:NoteGetFnMutex`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
