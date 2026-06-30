# `NoteSetFnSerialDefault`

## Status

Final. Behavior, public declaration, direct call sites, state mutation, locking, active-interface selection, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:356`
- Declaration/interface: `note.h:702`
- Public/exported names: `NoteSetFnSerialDefault`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFnSerialDefault(serialResetFn resetFn, serialTransmitFn transmitFn, serialAvailableFn availFn, serialReceiveFn receiveFn)`
- Visibility: public SDK hook-registration API
- Ownership/lifetime role: fills empty serial hook slots with caller-provided defaults; caller-provided functions must remain valid for later serial transactions

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:226:_LockNote` (calls, verified; enters Notecard hook mutex if configured)
- `function:n_hooks.c:214:_noteSetActiveInterface` (conditionally calls, verified; selects serial dispatch table when no interface is active)
- `macro:n_lib.h:227:_UnlockNote` (calls, verified; leaves Notecard hook mutex if configured)
- External/system dependencies: none
- Data/type/global dependencies: reads/writes `hookSerialReset`, `hookSerialTransmit`, `hookSerialAvailable`, `hookSerialReceive`, and reads `hookActiveInterface`
- Artifact coverage: generated artifact exists

## Behavior

Registers default serial transport hooks without overwriting existing serial hook slots. The function locks Notecard access, assigns each serial hook only if its current global slot is `NULL`, and calls `_noteSetActiveInterface(NOTE_C_INTERFACE_SERIAL)` only when `hookActiveInterface == NOTE_C_INTERFACE_NONE`.

This differs from `NoteSetFnSerial`: it is a fill-if-empty helper and preserves both existing hook pointers and an already-selected active interface.

## Important Boundaries

- Preconditions and assumptions: intended for adapter/platform defaults that should not override explicit application configuration
- Error/null/empty behavior: no return value and no validation; a `NULL` argument can be stored only when the corresponding hook slot is currently `NULL`
- Ownership and lifetime: stores function pointers only; it does not allocate or own hook implementations
- Concurrency/global state: protected by `_LockNote()`/`_UnlockNote()` while reading/writing hook globals and possibly changing active-interface dispatch
- Portability/platform constraints: keeps platform serial behavior behind caller-provided hooks
- Performance shape: constant-time conditional pointer assignment and optional dispatch selection
- Security or parsing constraints: no parsing; correctness depends on trusted hook implementations

## Direct Call Sites

No production library callers found by `rg -n "\bNoteSetFnSerialDefault\b" --glob '!source-research/**'`. References are the public declaration in `note.h:702`, the definition in `n_hooks.c:356`, and focused unit tests in `test/src/NoteSetFnSerialDefault_test.cpp`.

## Role In Callers

Adapter/application-facing default registration API. It installs serial defaults only where absent and activates serial dispatch only when no active interface has been selected.

## Tests Observed

`test/src/NoteSetFnSerialDefault_test.cpp` verifies that:

- empty serial hook globals are filled with provided defaults
- previously set hook globals are not changed by different defaults
- `_noteSetActiveInterface(NOTE_C_INTERFACE_SERIAL)` is called only when `hookActiveInterface` is `NOTE_C_INTERFACE_NONE`
- the Notecard lock/unlock functions are called once and balanced

## Graph Links

- Graph node id: `function:n_hooks.c:356:NoteSetFnSerialDefault`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

None for this node. Downstream serial transaction/reset/chunk behavior is covered by the active-interface dispatch targets and serial transport helper artifacts.
