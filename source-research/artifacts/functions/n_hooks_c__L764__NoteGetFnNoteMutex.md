# `NoteGetFnNoteMutex`

## Status

Finalized. Source-inspected public Notecard mutex hook getter.

## Identity

- Kind: function
- Definition: `n_hooks.c:764`
- Declaration/interface: `note.h:632`
- Public/exported names: `NoteGetFnNoteMutex`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteGetFnNoteMutex(mutexFn *lockFn, mutexFn *unlockFn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: copies current Notecard mutex hook pointers into caller-provided storage

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `hookLockNote` and `hookUnlockNote`
- Artifact coverage: source-inspected artifact

## Behavior

Writes `hookLockNote` to `*lockFn` if requested and writes `hookUnlockNote` to `*unlockFn` if requested. Null output pointers are skipped independently. The getter does not lock.

## Important Boundaries

- Preconditions and assumptions: non-null output pointers must point to writable storage for `mutexFn` values.
- Error/null/empty behavior: either output pointer may be null and is skipped; no return value or error path.
- Ownership and lifetime: copied callbacks remain owned by the caller/platform that installed them.
- Concurrency/global state: unsynchronized reads of global Notecard mutex hook pointers.
- Portability/platform constraints: this is the introspection boundary for platform Notecard mutex hooks.
- Performance shape: up to two null checks and pointer copies.
- Security or parsing constraints: no validation that copied callbacks remain valid after return.

## Direct Call Sites

Source/header exact search found only the public declaration in `note.h:632`, the definition in `n_hooks.c:764`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public hook-introspection API for downstream callers that need Notecard lock/unlock callback state.

## Tests Observed

Focused tests in `test/src/NoteGetFnNoteMutex_test.cpp` cover both outputs, all null outputs, and single-output cases. Setter/dispatch tests cover clearing behavior. Gap: getter-after-clear/default null state is not directly asserted by this getter test.

## Graph Links

- Graph node id: `function:n_hooks.c:764:NoteGetFnNoteMutex`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
