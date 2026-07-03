# `_noteChunkedTransmit`

## Status

Finalized. Source-inspected active-interface chunked transmit dispatcher, callback registrations, macro alias caller, and focused tests.

## Identity

- Kind: function
- Definition: `n_hooks.c:1087`
- Declaration/interface: `n_lib.h:151`
- Public/exported names: none; internal active-interface dispatch wrapper
- Macro aliases/wrappers: `_ChunkedTransmit` expands to `_noteChunkedTransmit` at `n_lib.h:240`
- Signature/API shape: `const char *_noteChunkedTransmit(const uint8_t *buffer, uint32_t size, bool delay)`
- Visibility: internal declaration in `n_lib.h`, implementation in `n_hooks.c`
- Ownership/lifetime role: forwards caller-owned buffer to the active chunked transmit callback; does not allocate or retain it

## Dependency Status

- Project-local dependencies:
  - Dynamic callback candidates through `notecardChunkedTransmit`:
    - `function:n_serial.c:303:_serialChunkedTransmit` finalized
    - `function:n_i2c.c:456:_i2cNoteChunkedTransmit` finalized
- External/system dependencies: transport behavior behind the active callback and platform hooks
- Data/type/global dependencies: reads `notecardChunkedTransmit` and `hookActiveInterface`; assignments are made by finalized `_noteSetActiveInterface`
- Artifact coverage: source, declaration, alias, callback registration paths, dynamic candidates, direct callers, and focused tests inspected

## Behavior

If `notecardChunkedTransmit` is null or `hookActiveInterface == NOTE_C_INTERFACE_NONE`, returns the literal error string `"a valid interface must be selected"`. Otherwise forwards `buffer`, `size`, and `delay` unchanged to the currently installed `notecardChunkedTransmit` callback and returns that callback's result.

## Important Boundaries

- Preconditions and assumptions: a serial or I2C active interface must have installed a chunked transmit callback for successful dispatch. Buffer validity is delegated to the active callback.
- Error/null/empty behavior: no active callback or `NOTE_C_INTERFACE_NONE` returns a literal error; otherwise all error/null/zero-size behavior is inherited from the active callback.
- Ownership and lifetime: no allocation and no ownership transfer; forwards the borrowed buffer pointer.
- Concurrency/global state: reads active-interface dispatch state; callers generally hold the relevant Notecard lock before binary transfers.
- Portability/platform constraints: platform transport behavior remains behind the selected serial or I2C adapter.
- Performance shape: O(1) dispatch plus active callback cost.
- Security or parsing constraints: no parsing; raw bytes are forwarded.

## Direct Call Sites

Exact source search found:

- `n_lib.h:151` internal declaration.
- `n_lib.h:240` `_ChunkedTransmit` macro alias.
- `n_hooks.c:1087` implementation.
- `n_helpers.c:683` `NoteBinaryStoreTransmit` uses the alias to send encoded binary data immediately after the setup request.

Related registration sites: `_noteSetActiveInterface` installs `_serialChunkedTransmit` for serial and `_i2cNoteChunkedTransmit` for I2C, and clears the callback for disabled/unrecognized interfaces.

## Role In Callers

`_ChunkedTransmit` is the direct alias used by binary-store transmit. Active-interface setup decides whether this dispatch reaches the serial adapter, the I2C adapter, or returns a no-interface error.

## Tests Observed

`test/src/_noteChunkedTransmit_test.cpp` covers `NOTE_C_INTERFACE_NONE` returning the interface error even when a callback is set, I2C/serial active interfaces with null callback returning the interface error, and I2C/serial active interfaces with callbacks forwarding parameters and returning the callback result. `_noteSetActiveInterface_test.cpp` verifies serial/I2C callback registration and callback clearing. `NoteBinaryStoreTransmit_test.cpp` fakes `_noteChunkedTransmit` to cover caller error propagation.

## Test Coverage Notes

Focused tests do not exercise the real serial/I2C callbacks through this dispatcher in one integration path; those adapters are covered by their own focused tests.

## Graph Links

- Graph node id: `function:n_hooks.c:1087:_noteChunkedTransmit`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
