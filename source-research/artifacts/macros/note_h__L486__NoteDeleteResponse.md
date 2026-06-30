# `NoteDeleteResponse`

## Status

Finalized. Source-inspected public response cleanup macro.

## Identity

- Kind: macro
- Definition: `note.h:486`
- Declaration/interface: public SDK header macro
- Signature/API shape: `NoteDeleteResponse(rsp)`
- Macro aliases/wrappers: expands to `JDelete(rsp)`
- Visibility: public SDK API
- Public/exported names: `NoteDeleteResponse`
- Ownership/lifetime role: releases a JSON response object returned by request APIs

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: JSON response type `J`; allocator/free hook state is owned by finalized `JDelete` closure
- Graph repair: added verified `NoteDeleteResponse` -> `JDelete` macro-alias edge after source inspection

## Behavior

This macro is a public semantic alias for `JDelete(rsp)`. It has no additional guards, logging, or ownership logic: the argument is evaluated as the argument to `JDelete`, and null behavior follows `JDelete`, whose loop returns immediately for `NULL`.

Through `JDelete`, freeing a response recursively releases child nodes, value strings, and key strings unless the corresponding reference/const flags are set, then frees each node through the SDK allocator hook.

## Important Boundaries

- Preconditions and assumptions: the pointer should be a `J *` response object or NULL.
- Error/null/empty behavior: NULL is accepted through `JDelete`; there is no error return.
- Ownership and lifetime: consumes the caller's response tree ownership; callers must not use the tree after this macro.
- Concurrency/global state: no lock behavior; allocator hook state is reached through `JDelete`.
- Performance shape: O(size of response tree), inherited from recursive/iterative JSON deletion.
- Portability/platform constraints: portable macro; allocator behavior remains behind note-c hooks.
- API boundary: gives response-cleanup call sites a public name distinct from general JSON deletion, but the implementation is exactly the JSON deleter.

## Direct Call Sites

Exact source search found the macro definition in `note.h` and response cleanup uses in `n_helpers.c`. Call sites occur in high-level helper APIs that obtain a `J *rsp` from `NoteRequestResponse`, extract values or branch on `NoteResponseError`, and then release the response:

- Time/location/status helpers: `NoteTimeST`, `NoteLocationValidST`, `NoteGetLocation`, `NoteGetLocationMode`, `NoteGetStatusST`
- Environment/connectivity/service helpers: `NoteGetEnv`, `NoteIsConnectedST`, `NoteGetNetStatus`, `NoteGetVersion`, `NoteGetServiceConfigST`
- Sleep/payload helpers: `NotePayloadRetrieveAfterSleep`
- Routing and sensor helpers: `NoteSendToRoute`, `NoteGetVoltage`, `NoteGetTemperature`, `NoteGetContact`
- Debug sync helper: `NoteDebugSyncStatus`

## Role In Callers

The macro marks the point where helper APIs finish with a response document. Some callers delete immediately on error, some after copying fields into caller buffers or static cache state, and `NoteSendToRoute` deletes the response after detaching its `"body"` child for reuse in a follow-on request.

## Tests Observed

No dedicated direct test targets the macro alias. Focused helper tests indirectly exercise response deletion paths where helpers fake or receive `NoteRequestResponse` results. `JDelete` has the finalized source behavior for the actual tree release semantics.

## Graph Links

- Graph node id: `macro:note.h:486:NoteDeleteResponse`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
