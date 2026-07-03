# `_SerialTransmit`

## Status

Final. Macro expansion target, behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: macro
- Definition: `n_lib.h:231`
- Declaration/interface: internal macro in `n_lib.h:231`
- Public/exported names: none; target helper `_noteSerialTransmit` is internal
- Macro aliases/wrappers: `_SerialTransmit` expands to `_noteSerialTransmit`
- Signature/API shape: `_SerialTransmit(buffer, len, flush)` macro returning void
- Visibility: internal transport macro
- Ownership/lifetime role: no ownership; aliases serial transmit dispatch helper

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:871:_noteSerialTransmit` (expands_to, verified)
- External/system dependencies: platform serial transmit hook through target helper
- Data/type/global dependencies: target reads `hookActiveInterface` and `hookSerialTransmit`
- Artifact coverage: generated artifact exists

## Behavior

Readability macro for `_noteSerialTransmit`. It inherits target behavior: active serial plus transmit hook forwards buffer/length/flush; inactive or missing hook is a no-op.

## Important Boundaries

- Preconditions and assumptions: serial interface and transmit hook should be configured before callers depend on output
- Error/null/empty behavior: no-op when inactive or hook missing; no error channel
- Ownership and lifetime: none
- Concurrency/global state: inherits target global hook/interface behavior
- Portability/platform constraints: platform transmit behavior remains behind hook
- Performance shape: preprocessor alias to function call
- Security or parsing constraints: no parsing; buffer safety depends on caller and hook honoring length

## Direct Call Sites

Verified direct production callers: `n_serial.c:58`, `n_serial.c:176`, and `n_serial.c:321`.

## Role In Callers

Serial transmit primitive used by serial transaction, reset, and chunked transmit paths.

## Tests Observed

Focused evidence comes through `_noteSerialTransmit_test.cpp`; higher-level serial tests fake `_noteSerialTransmit` for transaction/reset/chunk behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:231:_SerialTransmit`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this macro.
