# `_noteSerialTransmit`

## Status

Final. Behavior, internal declaration, hook dispatch, no-op behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:871`
- Declaration/interface: `n_lib.h:142`
- Public/exported names: internal `_noteSerialTransmit`; public hook setters are `NoteSetFnSerial` and `NoteSetFnSerialDefault`
- Macro aliases/wrappers: `_SerialTransmit` expands to `_noteSerialTransmit` at `n_lib.h:231`
- Signature/API shape: `void _noteSerialTransmit(const uint8_t *text, size_t len, bool flush)`
- Visibility: internal serial transmit dispatch helper
- Ownership/lifetime role: forwards caller-owned buffer to platform hook; no ownership

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: platform serial transmit hook
- Data/type/global dependencies: reads `hookActiveInterface` and `hookSerialTransmit`
- Artifact coverage: generated artifact exists

## Behavior

If `hookActiveInterface == NOTE_C_INTERFACE_SERIAL` and `hookSerialTransmit != NULL`, forwards the buffer, length, and flush flag to the registered transmit hook. Otherwise it does nothing. The implementation intentionally casts away `const` on `text` because `serialTransmitFn` takes `uint8_t *` for API compatibility.

## Important Boundaries

- Preconditions and assumptions: serial interface and transmit hook must be configured for output to occur
- Error/null/empty behavior: inactive interface or missing hook is a silent no-op; no error channel
- Ownership and lifetime: does not allocate or retain `text`
- Concurrency/global state: reads global active-interface and hook state without local locking
- Portability/platform constraints: platform transmit behavior stays behind hook
- Performance shape: constant-time gate plus platform hook call
- Security or parsing constraints: no parsing; hook must honor buffer/length contract

## Direct Call Sites

Direct internal macro callers use `_SerialTransmit` in `n_serial.c:58`, `n_serial.c:176`, and `n_serial.c:321`. Focused direct tests are in `test/src/_noteSerialTransmit_test.cpp`.

## Role In Callers

Serial transmit dispatch primitive for transaction newline flush, reset sync, and chunked transmit paths.

## Tests Observed

`test/src/_noteSerialTransmit_test.cpp` verifies inactive interface or missing hook do nothing, and active serial plus a hook forwards buffer, size, and flush parameters.

## Graph Links

- Graph node id: `function:n_hooks.c:871:_noteSerialTransmit`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers hook mutation of the const source buffer; source shows the cast is intentional for API compatibility.
