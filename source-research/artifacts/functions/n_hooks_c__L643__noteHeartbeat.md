# `_noteHeartbeat`

## Status

Finalized. Source-inspected optional heartbeat callback dispatcher.

## Identity

- Kind: function
- Definition: `n_hooks.c:643`
- Declaration/interface: compiled only under `NOTE_C_HEARTBEAT_CALLBACK`
- Signature/API shape: `bool _noteHeartbeat(const char *heartbeatJson)`
- Visibility: internal hook helper when heartbeat callback support is enabled
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `hookHeartbeat`, `hookHeartbeatContext`

## Behavior

If a heartbeat callback is registered, calls `hookHeartbeat(heartbeatJson, hookHeartbeatContext)` and returns that result. If no callback is registered, returns false.

## Important Boundaries

- Preconditions and assumptions: only compiled when `NOTE_C_HEARTBEAT_CALLBACK` is defined.
- Error/null/empty behavior: no callback means heartbeat is not abandoned.
- Ownership and lifetime: no allocation; passes borrowed heartbeat status/string.
- Concurrency/global state: reads global callback pointer and context.
- Performance shape: O(1) plus callback cost.
- Portability/platform constraints: excluded unless heartbeat callback feature is enabled.

## Direct Call Sites

Exact source search found `_noteTransactionShouldLock` as the transaction caller under `NOTE_C_HEARTBEAT_CALLBACK`.

## Role In Callers

The transaction retry loop uses a true return value to abandon the transaction, mark reset required, and return a heartbeat error.

## Tests Observed

`_noteHeartbeat_test.cpp` covers callback dispatch; `NoteTransaction_test.cpp` covers heartbeat integration.

## Graph Links

- Graph node id: `function:n_hooks.c:643:_noteHeartbeat`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
