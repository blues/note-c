# `NoteIsConnected`

## Status

Finalized. Source-inspected public immediate-refresh connectivity wrapper, direct caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_helpers.c:1334`
- Declaration/interface: `note.h:1844`
- Public/exported names: `NoteIsConnected`
- Macro aliases/wrappers: none
- Signature/API shape: `bool NoteIsConnected(void)`
- Visibility: public note-c helper API
- Ownership/lifetime role: no caller-owned data; delegates response ownership to `NoteIsConnectedST`

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:1346:NoteIsConnectedST` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes static `connectivityTimer`
- Artifact coverage: source, declaration, direct caller search, and focused tests inspected

## Behavior

Sets the static `connectivityTimer` to zero, then returns `NoteIsConnectedST()`. Because `_timerExpiredSecs` treats a zero previous timer as expired, this wrapper forces an immediate connectivity refresh attempt instead of using the suppressed cached path.

## Important Boundaries

- Preconditions and assumptions: global helper state is available; normal callers do not pass arguments.
- Error/null/empty behavior: error behavior is delegated to `NoteIsConnectedST`; this wrapper resets only the timer and does not clear the cached `cardConnected` value.
- Ownership and lifetime: no allocation in this wrapper.
- Concurrency/global state: mutates static `connectivityTimer` without local locking.
- Portability/platform constraints: transport and timing behavior remain behind `NoteIsConnectedST`.
- Performance shape: one forced `NoteIsConnectedST` refresh attempt.
- Security or parsing constraints: no parsing in this wrapper.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This is the public non-suppressed connectivity helper API.

## Tests Observed

`test/src/NoteIsConnected_test.cpp` covers no response, error response, connected false, and connected true cases through this public wrapper.

## Test Coverage Notes

Focused tests do not assert that `connectivityTimer` was reset, do not verify the request name passed to `NoteNewRequest`, and do not cover cached true value preservation across later null/error responses.

## Graph Links

- Graph node id: `function:n_helpers.c:1334:NoteIsConnected`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
