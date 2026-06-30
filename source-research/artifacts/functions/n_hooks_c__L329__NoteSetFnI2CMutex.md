# `NoteSetFnI2CMutex`

## Status

Final. Behavior, public declaration, global state mutation, direct call sites, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:329`
- Declaration/interface: `note.h:605`
- Public/exported names: `NoteSetFnI2CMutex`
- Macro aliases/wrappers: unverified
- Signature/API shape: `void NoteSetFnI2CMutex(mutexFn lockI2Cfn, mutexFn unlockI2Cfn)`
- Visibility: public SDK hook-registration API
- Ownership/lifetime role: stores caller-provided I2C mutex hook function pointers in globals

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none
- Data/type/global dependencies: writes `hookLockI2C` and `hookUnlockI2C`
- Artifact coverage: generated artifact exists

## Behavior

Installs or clears the I2C bus lock/unlock hooks by assigning `hookLockI2C = lockI2Cfn` and `hookUnlockI2C = unlockI2Cfn`.

## Important Boundaries

- Preconditions and assumptions: platform functions must remain valid while registered
- Error/null/empty behavior: `NULL` arguments are accepted and clear the corresponding hook
- Ownership and lifetime: stores function pointers only
- Concurrency/global state: mutates global hook pointers without locking
- Portability/platform constraints: keeps platform mutex behavior behind hooks
- Performance shape: constant-time pointer assignment
- Security or parsing constraints: no parsing; trusts hook implementations

## Direct Call Sites

No production library callers found outside definition/declaration. Focused tests call it in `test/src/NoteSetFnI2CMutex_test.cpp`; `NoteGetFnI2CMutex_test.cpp` also uses it to configure/get hooks.

## Role In Callers

Application/adapter registration API for I2C bus locking hooks consumed by `NoteLockI2C` and `NoteUnlockI2C`.

## Tests Observed

`test/src/NoteSetFnI2CMutex_test.cpp` verifies installed hooks are called once by `NoteLockI2C`/`NoteUnlockI2C`, and setting both hooks to `NULL` makes subsequent lock/unlock calls no-ops.

## Graph Links

- Graph node id: `function:n_hooks.c:329:NoteSetFnI2CMutex`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
