# `NoteSetFnDisabled`

## Status

Finalized. Source-inspected public communication-disable setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:433`
- Declaration/interface: `note.h:789`
- Signature/API shape: `void NoteSetFnDisabled(void)`
- Visibility: public SDK hook API
- Public/exported names: `NoteSetFnDisabled`
- Macro aliases/wrappers: none
- Ownership/lifetime role: no ownership; lock-protected active-interface disable operation

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `function:n_hooks.c:214:_noteSetActiveInterface` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: Notecard lock hook state and active-interface dispatch globals through callees

## Behavior

Takes the Notecard lock, calls `_noteSetActiveInterface(NOTE_C_INTERFACE_NONE)`, and releases the lock. The dispatch selector clears active transport function pointers and records the active interface as none.

## Important Boundaries

- Preconditions and assumptions: none.
- Error/null/empty behavior: no return value and no error signal.
- Ownership and lifetime: no allocation.
- Concurrency/global state: lock-protects transition to disabled interface state.
- Performance shape: O(1) plus lock hook cost.
- Portability/platform constraints: disables communication without changing platform hook storage directly.

## Direct Call Sites

Exact source search found public declaration, implementation, and focused tests. No production library caller was found; this is a public API entry point.

## Role In Callers

Allows applications/adapters to disable the active communication interface so later dynamic-dispatch transactions/chunk operations see no selected interface.

## Tests Observed

`test/src/NoteSetFnDisabled_test.cpp` verifies `_noteSetActiveInterface(NOTE_C_INTERFACE_NONE)` and balanced lock/unlock calls.

## Graph Links

- Graph node id: `function:n_hooks.c:433:NoteSetFnDisabled`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
