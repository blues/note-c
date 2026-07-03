# `NoteTransaction`

## Status

Finalized. Source-inspected public non-consuming transaction wrapper.

## Identity

- Kind: function
- Definition: `n_request.c:411`
- Declaration/interface: `note.h:500`
- Signature/API shape: `J *NoteTransaction(J *req)`
- Visibility: public SDK API
- Public/exported names: `NoteTransaction`
- Macro aliases/wrappers: none
- Ownership/lifetime role: caller retains ownership of `req`; returned `J *` is caller-owned when non-null

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:428:_noteTransactionShouldLock` finalized
- External/system dependencies: none
- Data/type/global dependencies: transaction, locking, reset, CRC, logging, and active-interface state are owned by `_noteTransactionShouldLock`

## Behavior

Delegates directly to `_noteTransactionShouldLock(req, true)`.

Passing `lockNotecard=true` makes the transaction orchestrator acquire and release the Notecard lock for the call. All request validation, JSON serialization, user-agent handling, reset handling, transport dispatch, retry classification, response parsing, command empty-object behavior, CRC behavior, and error-document behavior are inherited from the finalized orchestrator.

## Important Boundaries

- Preconditions and assumptions: caller owns `req`; the wrapper does not delete it.
- Error/null/empty behavior: null, malformed, transport, reset, parse, and response-error behavior follows `_noteTransactionShouldLock`.
- Ownership and lifetime: does not consume `req`; transfers returned response ownership to the caller.
- Concurrency/global state: uses the orchestrator with `lockNotecard=true`.
- Performance shape: O(transaction orchestration), inherited from `_noteTransactionShouldLock`.
- Portability/platform constraints: transport, reset, timing, lock, allocation, and debug behavior stay behind note-c hooks and orchestrator dependencies.

## Direct Call Sites

Exact source search found production callers in `NoteRequestResponse` and `NoteRequestResponseWithRetry`. Tests call `NoteTransaction` directly in `NoteTransaction_test.cpp` and timeout/CRC-focused tests.

## Role In Callers

`NoteRequestResponse` uses this wrapper then deletes the request object itself. `NoteRequestResponseWithRetry` repeatedly calls it until success, non-retryable response, or timeout, deleting retry responses between attempts.

## Tests Observed

`NoteTransaction_test.cpp` directly exercises the public wrapper over the full orchestrator behavior, including null/malformed requests, transaction start, lock handling, reset, retry, invalid JSON, CRC, user-agent, command, and heartbeat branches. `_noteTransaction_calculateTimeoutMs_test.cpp` invokes `NoteTransaction` while focused on timeout selection.

## Graph Links

- Graph node id: `function:n_request.c:411:NoteTransaction`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
