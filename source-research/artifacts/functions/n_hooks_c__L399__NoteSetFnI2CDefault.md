# `NoteSetFnI2CDefault`

## Status

Final. Behavior, public declaration, direct call sites, state mutation, locking, active-interface selection, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:399`
- Declaration/interface: `note.h:748`
- Public/exported names: `NoteSetFnI2CDefault`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFnI2CDefault(uint32_t notecardAddr, uint32_t maxTransmitSize, i2cResetFn resetFn, i2cTransmitFn transmitFn, i2cReceiveFn receiveFn)`
- Visibility: public SDK hook-registration API
- Ownership/lifetime role: fills empty I2C address/MTU/hook slots with caller-provided defaults; caller-provided functions must remain valid for later I2C transactions

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:226:_LockNote` (calls, verified; enters Notecard hook mutex if configured)
- `function:n_hooks.c:214:_noteSetActiveInterface` (conditionally calls, verified; selects I2C dispatch table when no interface is active)
- `macro:n_lib.h:227:_UnlockNote` (calls, verified; leaves Notecard hook mutex if configured)
- External/system dependencies: none
- Data/type/global dependencies: reads/writes `i2cAddress`, `i2cMax`, `hookI2CReset`, `hookI2CTransmit`, `hookI2CReceive`, and reads `hookActiveInterface`
- Artifact coverage: generated artifact exists

## Behavior

Registers default I2C transport configuration without overwriting existing I2C state. The function locks Notecard access, assigns `i2cAddress` only when it is `0`, assigns `i2cMax` only when it is `0`, assigns each I2C hook only if its current global slot is `NULL`, and calls `_noteSetActiveInterface(NOTE_C_INTERFACE_I2C)` only when `hookActiveInterface == NOTE_C_INTERFACE_NONE`.

This differs from `NoteSetFnI2C`: it is a fill-if-empty helper and preserves both existing I2C values/hook pointers and an already-selected active interface.

## Important Boundaries

- Preconditions and assumptions: intended for adapter/platform defaults that should not override explicit application configuration
- Error/null/empty behavior: no return value and no validation; zero address/MTU or `NULL` hook arguments can be stored only when the corresponding current slot is empty
- Ownership and lifetime: stores function pointers only; it does not allocate or own hook implementations
- Concurrency/global state: protected by `_LockNote()`/`_UnlockNote()` while reading/writing I2C globals and possibly changing active-interface dispatch
- Portability/platform constraints: keeps platform I2C behavior behind caller-provided hooks
- Performance shape: constant-time conditional scalar/pointer assignment and optional dispatch selection
- Security or parsing constraints: no parsing; correctness depends on trusted hook implementations

## Direct Call Sites

No production library callers found by `rg -n "\bNoteSetFnI2CDefault\b" --glob '!source-research/**'`. References are the public declaration in `note.h:748`, the definition in `n_hooks.c:399`, and focused unit tests in `test/src/NoteSetFnI2CDefault_test.cpp`.

## Role In Callers

Adapter/application-facing default registration API. It installs I2C defaults only where absent and activates I2C dispatch only when no active interface has been selected.

## Tests Observed

`test/src/NoteSetFnI2CDefault_test.cpp` verifies that:

- empty I2C address/MTU/hook globals are filled with provided defaults
- previously set address, MTU, and hook globals are not changed by different defaults
- `_noteSetActiveInterface(NOTE_C_INTERFACE_I2C)` is called only when `hookActiveInterface` is `NOTE_C_INTERFACE_NONE`
- the Notecard lock/unlock functions are called once and balanced

## Graph Links

- Graph node id: `function:n_hooks.c:399:NoteSetFnI2CDefault`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

None for this node. Downstream I2C transaction/reset/chunk behavior is covered by the active-interface dispatch targets and I2C transport helper artifacts.
