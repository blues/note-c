# `NoteGetFnHeartbeat`

## Status

Finalized. Source-inspected conditional public heartbeat hook getter.

## Identity

- Kind: function
- Definition: `n_hooks.c:714`
- Declaration/interface: `note.h:554`
- Public/exported names: `NoteGetFnHeartbeat` when `NOTE_C_HEARTBEAT_CALLBACK` is enabled
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteGetFnHeartbeat(heartbeatFn *fn, void **context)`
- Visibility: conditional public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: copies the current heartbeat callback and context values into caller-provided storage

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `hookHeartbeat` and `hookHeartbeatContext`
- Artifact coverage: source-inspected artifact

## Behavior

When compiled with `NOTE_C_HEARTBEAT_CALLBACK`, writes `hookHeartbeat` to `*fn` if `fn` is non-null and writes `hookHeartbeatContext` to `*context` if `context` is non-null. Output slots are handled independently. The getter does not lock.

## Important Boundaries

- Preconditions and assumptions: non-null output pointers must point to writable storage for their matching types.
- Error/null/empty behavior: either output pointer may be null and is skipped; no return value or error path.
- Ownership and lifetime: copied callback/context remain owned by the caller/platform that installed them.
- Concurrency/global state: unsynchronized reads of heartbeat globals.
- Portability/platform constraints: symbol exists only in `NOTE_C_HEARTBEAT_CALLBACK` builds.
- Performance shape: up to two null checks and pointer copies.
- Security or parsing constraints: no validation that copied callback/context remain valid after return.

## Direct Call Sites

Source/header exact search found only the conditional public declaration in `note.h:554`, the conditional definition in `n_hooks.c:714`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public hook-introspection API for downstream callers that need heartbeat callback state in heartbeat-enabled builds.

## Tests Observed

Focused tests in `test/src/NoteGetFnHeartbeat_test.cpp` cover null output arguments, single-output arguments, full round-trip, and all-null stored state. `test/src/_noteHeartbeat_test.cpp` covers dispatch wrapper behavior. Gap: non-null context with null heartbeat callback is not directly asserted.

## Graph Links

- Graph node id: `function:n_hooks.c:714:NoteGetFnHeartbeat`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
