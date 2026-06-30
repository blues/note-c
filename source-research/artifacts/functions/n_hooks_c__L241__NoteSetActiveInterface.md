# `NoteSetActiveInterface`

## Status

Finalized. Source-inspected public active-interface setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:241`
- Declaration/interface: `note.h:777`
- Signature/API shape: `void NoteSetActiveInterface(int interface)`
- Visibility: public SDK hook API
- Public/exported names: `NoteSetActiveInterface`
- Macro aliases/wrappers: none
- Ownership/lifetime role: no ownership; synchronizes global active-interface dispatch update

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `function:n_hooks.c:214:_noteSetActiveInterface` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: Notecard lock hook state and active-interface dispatch globals through callees

## Behavior

Takes the Notecard lock with `_LockNote`, calls `_noteSetActiveInterface(interface)`, then releases the lock with `_UnlockNote`.

The selected serial/I2C/disabled dispatch behavior is owned by `_noteSetActiveInterface`; this wrapper adds synchronization for public callers.

## Important Boundaries

- Preconditions and assumptions: accepts any integer; unrecognized values are normalized to disabled state by the helper.
- Error/null/empty behavior: no return value and no error signal.
- Ownership and lifetime: no allocation.
- Concurrency/global state: serializes active-interface dispatch mutation with the Notecard lock when lock hooks are installed.
- Performance shape: O(1) plus lock hook cost.
- Portability/platform constraints: active interface selection remains portable; concrete I/O remains behind selected adapter functions and hooks.

## Direct Call Sites

Exact source search found public declaration, implementation, and focused tests. No production library caller was found; this is a public API entry point.

## Role In Callers

Allows applications/adapters to switch the active Notecard communication interface explicitly while ensuring the global dispatch table update is lock-protected.

## Tests Observed

`test/src/NoteSetActiveInterface_test.cpp` verifies that NONE, I2C, and SERIAL values are forwarded to `_noteSetActiveInterface` and that lock/unlock calls are balanced.

## Graph Links

- Graph node id: `function:n_hooks.c:241:NoteSetActiveInterface`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
