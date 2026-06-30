# `NoteSetFnI2C`

## Status

Final. Behavior, public declaration, direct call sites, state mutation, locking, active-interface selection, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:381`
- Declaration/interface: `note.h:731`
- Public/exported names: `NoteSetFnI2C`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFnI2C(uint32_t notecardAddr, uint32_t maxTransmitSize, i2cResetFn resetFn, i2cTransmitFn transmitFn, i2cReceiveFn receiveFn)`
- Visibility: public SDK hook-registration API
- Ownership/lifetime role: stores caller-provided I2C address/MTU values and hook function pointers in process-global slots; caller-provided functions must remain valid for later I2C transactions

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:226:_LockNote` (calls, verified; enters Notecard hook mutex if configured)
- `function:n_hooks.c:214:_noteSetActiveInterface` (calls, verified; selects I2C dispatch table)
- `macro:n_lib.h:227:_UnlockNote` (calls, verified; leaves Notecard hook mutex if configured)
- External/system dependencies: none
- Data/type/global dependencies: writes `i2cAddress`, `i2cMax`, `hookI2CReset`, `hookI2CTransmit`, and `hookI2CReceive`
- Artifact coverage: generated artifact exists

## Behavior

Registers an explicit I2C transport implementation. The function locks Notecard access, overwrites `i2cAddress` and `i2cMax`, overwrites all three I2C hook globals with the caller-provided pointers, calls `_noteSetActiveInterface(NOTE_C_INTERFACE_I2C)`, and unlocks.

This is an unconditional setter: passing zero address/MTU or `NULL` hooks replaces prior values, and the active interface is still switched to I2C.

## Important Boundaries

- Preconditions and assumptions: intended for platform/adapter initialization before I2C transactions use the hooks
- Error/null/empty behavior: no return value and no validation; zero address/MTU and `NULL` hook arguments are stored as current values
- Ownership and lifetime: stores function pointers only; it does not allocate, copy pointed-to code, or take ownership beyond retaining pointer values
- Concurrency/global state: protected by `_LockNote()`/`_UnlockNote()` while mutating I2C globals and active-interface dispatch
- Portability/platform constraints: keeps platform I2C behavior behind caller-provided hooks
- Performance shape: constant-time scalar/pointer assignment and dispatch selection
- Security or parsing constraints: no parsing; correctness depends on trusted hook implementations

## Direct Call Sites

No production library callers found by `rg -n "\bNoteSetFnI2C\b" --glob '!source-research/**'`. References are the public declaration in `note.h:731`, the definition in `n_hooks.c:381`, and focused unit tests in `test/src/NoteSetFnI2C_test.cpp`.

## Role In Callers

Adapter/application-facing registration API. It moves future Notecard transactions onto the I2C dispatch path by delegating to `_noteSetActiveInterface(NOTE_C_INTERFACE_I2C)`.

## Tests Observed

`test/src/NoteSetFnI2C_test.cpp` verifies that:

- unset I2C state and hook globals are set to the provided values/functions
- previously set address, MTU, and hook globals are overwritten, including by zero/`NULL`
- `_noteSetActiveInterface` receives `NOTE_C_INTERFACE_I2C`
- the Notecard lock/unlock functions are called once and balanced

## Graph Links

- Graph node id: `function:n_hooks.c:381:NoteSetFnI2C`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

None for this node. Downstream I2C transaction/reset/chunk behavior is covered by the active-interface dispatch targets and I2C transport helper artifacts.
