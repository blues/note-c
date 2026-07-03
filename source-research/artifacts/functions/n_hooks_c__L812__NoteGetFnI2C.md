# `NoteGetFnI2C`

## Status

Finalized. Source-inspected public I2C hook/address/MTU getter.

## Identity

- Kind: function
- Definition: `n_hooks.c:812`
- Declaration/interface: `note.h:763`
- Public/exported names: `NoteGetFnI2C`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteGetFnI2C(uint32_t *notecardAddr, uint32_t *maxTransmitSize, i2cResetFn *resetFn, i2cTransmitFn *transmitFn, i2cReceiveFn *receiveFn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: copies current raw I2C address/MTU values and hook pointers into caller-provided storage

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads `i2cAddress`, `i2cMax`, `hookI2CReset`, `hookI2CTransmit`, and `hookI2CReceive`
- Artifact coverage: source-inspected artifact

## Behavior

Locks Notecard access, conditionally copies raw `i2cAddress`, raw `i2cMax`, and current I2C hook pointers into non-null output slots, then unlocks. It does not apply default address or MTU clamping.

## Important Boundaries

- Preconditions and assumptions: non-null output pointers must point to writable storage for matching value/function-pointer types.
- Error/null/empty behavior: any output pointer may be null; all-null calls still lock and unlock.
- Ownership and lifetime: copied callbacks remain owned by the caller/platform that installed them.
- Concurrency/global state: serializes reads with `_LockNote`/`_UnlockNote`.
- Portability/platform constraints: this is the introspection boundary for platform I2C hook state.
- Performance shape: one lock, up to five copies, one unlock.
- Security or parsing constraints: no validation that copied callbacks remain valid after return.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:763`, the definition in `n_hooks.c:812`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public hook-introspection API for downstream callers that need current I2C raw configuration and callbacks.

## Tests Observed

Focused tests in `test/src/NoteGetFnI2C_test.cpp` cover all-null, all outputs, each single output, and lock/unlock counts. Gap: tests cover raw stored values, not defaulted/clamped values from `NoteI2CAddress` or `NoteI2CMax`.

## Graph Links

- Graph node id: `function:n_hooks.c:812:NoteGetFnI2C`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockNote` and `_UnlockNote`.

## Evidence Gaps

No unresolved source gaps for this node.
