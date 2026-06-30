# `_noteSetActiveInterface`

## Status

Finalized. Source-inspected active-interface dispatch selector.

## Identity

- Kind: function
- Definition: `n_hooks.c:214`
- Declaration/interface: `test/include/test_static.h:25` when static helpers are test-visible
- Signature/API shape: `NOTE_C_STATIC void _noteSetActiveInterface(int interface)`
- Visibility: internal hook helper
- Public/exported names: none
- Macro aliases/wrappers: none
- Ownership/lifetime role: mutates global active-interface and dispatch function-pointer state

## Dependency Status

- Project-local dependencies: no project-local callees
- External/system dependencies: none
- Data/type/global dependencies: `hookActiveInterface`, `notecardReset`, `notecardTransaction`, `notecardChunkedReceive`, `notecardChunkedTransmit`, serial adapter entry points, I2C adapter entry points

## Behavior

Stores the requested interface in `hookActiveInterface`, then selects the active Notecard transport dispatch table.

- `NOTE_C_INTERFACE_SERIAL`: sets reset/transaction/chunked receive/chunked transmit dispatch pointers to `_serialNoteReset`, `_serialNoteTransaction`, `_serialChunkedReceive`, and `_serialChunkedTransmit`.
- `NOTE_C_INTERFACE_I2C`: sets dispatch pointers to `_i2cNoteReset`, `_i2cNoteTransaction`, `_i2cNoteChunkedReceive`, and `_i2cNoteChunkedTransmit`.
- Any other value, including `NOTE_C_INTERFACE_NONE`: normalizes `hookActiveInterface` to `NOTE_C_INTERFACE_NONE` and clears all four dispatch pointers to NULL.

The function only assigns pointers; it does not call the selected transport functions.

## Important Boundaries

- Preconditions and assumptions: caller supplies one of the public interface constants or accepts disabled behavior for any other value.
- Error/null/empty behavior: unrecognized values silently disable active communication.
- Ownership and lifetime: no allocation; stores addresses of static adapter functions.
- Concurrency/global state: mutates global dispatch state. Public callers wrap this helper with the Notecard lock; tests call it directly.
- Performance shape: O(1).
- Portability/platform constraints: centralizes active serial/I2C adapter selection while platform-specific I/O remains in adapter functions and hooks.

## Direct Call Sites

Exact source search found production callers in `NoteSetActiveInterface`, `NoteSetFnSerial`, `NoteSetFnSerialDefault`, `NoteSetFnI2C`, `NoteSetFnI2CDefault`, and `NoteSetFnDisabled`.

## Role In Callers

Public setter APIs install or preserve hook pointers, then use this helper to choose which transport dispatch table active transactions will use. Default setter variants only call it when no interface is active.

## Tests Observed

`test/src/_noteSetActiveInterface_test.cpp` verifies NULL/previously-set dispatch state for none, I2C, serial, and unrecognized interface values. Public setter tests fake `_noteSetActiveInterface` to verify callers pass the intended interface or skip default activation when an interface is already active.

## Graph Links

- Graph node id: `function:n_hooks.c:214:_noteSetActiveInterface`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
