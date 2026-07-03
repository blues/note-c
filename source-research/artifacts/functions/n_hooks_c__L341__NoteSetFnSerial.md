# `NoteSetFnSerial`

## Status

Final. Behavior, public declaration, direct call sites, state mutation, locking, active-interface selection, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:341`
- Declaration/interface: `note.h:687`
- Public/exported names: `NoteSetFnSerial`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFnSerial(serialResetFn resetFn, serialTransmitFn transmitFn, serialAvailableFn availFn, serialReceiveFn receiveFn)`
- Visibility: public SDK hook-registration API
- Ownership/lifetime role: stores caller-provided serial hook function pointers in process-global hook slots; caller-provided functions must remain valid for later serial transactions

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:226:_LockNote` (calls, verified; enters Notecard hook mutex if configured)
- `function:n_hooks.c:214:_noteSetActiveInterface` (calls, verified; selects serial dispatch table)
- `macro:n_lib.h:227:_UnlockNote` (calls, verified; leaves Notecard hook mutex if configured)
- External/system dependencies: none
- Data/type/global dependencies: writes `hookSerialReset`, `hookSerialTransmit`, `hookSerialAvailable`, and `hookSerialReceive`
- Artifact coverage: generated artifact exists

## Behavior

Registers an explicit serial transport implementation. The function locks Notecard access, overwrites all four serial hook globals with the caller-provided pointers, calls `_noteSetActiveInterface(NOTE_C_INTERFACE_SERIAL)`, and unlocks.

This is an unconditional setter: passing `NULL` replaces any previously registered serial hook with `NULL`, and the active interface is still switched to serial.

## Important Boundaries

- Preconditions and assumptions: intended for platform/adapter initialization before serial transactions use the hooks
- Error/null/empty behavior: no return value and no validation; `NULL` arguments are stored as hook values
- Ownership and lifetime: stores function pointers only; it does not allocate, copy pointed-to code, or take ownership beyond retaining the pointer values
- Concurrency/global state: protected by `_LockNote()`/`_UnlockNote()` while mutating hook globals and active-interface dispatch
- Portability/platform constraints: keeps platform serial behavior behind caller-provided hooks
- Performance shape: constant-time pointer assignment and dispatch selection
- Security or parsing constraints: no parsing; correctness depends on trusted hook implementations

## Direct Call Sites

No production library callers found by `rg -n "\bNoteSetFnSerial\b" --glob '!source-research/**'`. References are the public declaration in `note.h:687`, the definition in `n_hooks.c:341`, and focused unit tests in `test/src/NoteSetFnSerial_test.cpp`.

## Role In Callers

Adapter/application-facing registration API. It moves future Notecard transactions onto the serial dispatch path by delegating to `_noteSetActiveInterface(NOTE_C_INTERFACE_SERIAL)`.

## Tests Observed

`test/src/NoteSetFnSerial_test.cpp` verifies that:

- unset hook globals are set to the provided functions
- previously set hook globals are overwritten, including by `NULL`
- `_noteSetActiveInterface` receives `NOTE_C_INTERFACE_SERIAL`
- the Notecard lock/unlock functions are called once and balanced

## Graph Links

- Graph node id: `function:n_hooks.c:341:NoteSetFnSerial`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

None for this node. Downstream serial transaction/reset/chunk behavior is covered by the active-interface dispatch targets and serial transport helper artifacts.
