# `NoteLockI2C`

## Status

Final. Behavior, public declaration, macro alias, hook dependency, direct call sites, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:616`
- Declaration/interface: `note.h:1073`
- Public/exported names: `NoteLockI2C`
- Macro aliases/wrappers: `_LockI2C` expands to `NoteLockI2C` at `n_lib.h:245`
- Signature/API shape: `void NoteLockI2C(void)`
- Visibility: public SDK function and internal macro target
- Ownership/lifetime role: invokes registered lock hook; no ownership

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: platform lock hook supplied by application/adapter
- Data/type/global dependencies: reads `hookLockI2C`
- Artifact coverage: generated artifact exists

## Behavior

Calls `hookLockI2C()` when that global hook is non-`NULL`; otherwise it returns without action.

## Important Boundaries

- Preconditions and assumptions: registered hook should implement any required platform locking
- Error/null/empty behavior: unset hook is a no-op; no error channel
- Ownership and lifetime: no allocation or ownership transfer
- Concurrency/global state: reads a global function pointer without synchronization
- Portability/platform constraints: platform-specific behavior stays behind the hook
- Performance shape: constant-time null check plus hook call when set
- Security or parsing constraints: no parsing; trusts hook implementation

## Direct Call Sites

Direct internal macro callers use `_LockI2C` in `n_i2c.c` and `n_request.c`. Direct test/fake references include `NoteSetFnI2CMutex_test.cpp`, `NoteSetFnMutex_test.cpp`, `NotePing_test.cpp`, `_i2cNoteTransaction_test.cpp`, `_i2cNoteReset_test.cpp`, `_i2cNoteChunkedReceive_test.cpp`, and `_i2cNoteChunkedTransmit_test.cpp`.

## Role In Callers

I2C bus lock primitive for transaction/reset/chunk wrappers.

## Tests Observed

`test/src/NoteSetFnI2CMutex_test.cpp` verifies call-through when the hook is set and no-op behavior after clearing hooks. I2C transport wrapper tests verify lock/unlock balance through faked `NoteLockI2C`.

## Graph Links

- Graph node id: `function:n_hooks.c:616:NoteLockI2C`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
