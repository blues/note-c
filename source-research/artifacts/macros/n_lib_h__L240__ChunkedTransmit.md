# `_ChunkedTransmit`

## Status

Finalized. Source-inspected chunked transmit alias, finalized active-interface dispatcher target, direct caller, and focused target/caller tests.

## Identity

- Kind: macro
- Definition: `n_lib.h:240`
- Declaration/interface: internal readability wrapper in `n_lib.h`
- Public/exported names: none
- Macro aliases/wrappers: expands directly to `_noteChunkedTransmit`
- Signature/API shape: `_ChunkedTransmit`
- Visibility: project-internal macro for translation units including `n_lib.h`
- Ownership/lifetime role: no allocation or ownership transfer; aliases active-interface chunked transmit dispatch

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:1087:_noteChunkedTransmit` finalized
- External/system dependencies: serial/I2C platform transmit behavior through the dispatcher-selected callback
- Data/type/global dependencies: inherited from `_noteChunkedTransmit`: active-interface state and `notecardChunkedTransmit`
- Artifact coverage: macro definition, finalized expansion target, direct caller, registration paths, and focused tests inspected

## Behavior

Expands directly to `_noteChunkedTransmit`. Behavior is inherited from the finalized target: no installed callback or `NOTE_C_INTERFACE_NONE` returns `"a valid interface must be selected"`; otherwise the target forwards `buffer`, `size`, and `delay` to the active serial or I2C chunked transmit callback and returns that callback's result.

## Important Boundaries

- Preconditions and assumptions: active interface must be configured for successful dispatch; buffer validity is delegated to the selected transport callback.
- Error/null/empty behavior: inherited from the dispatcher and active callback.
- Ownership and lifetime: no allocation or ownership transfer; forwards borrowed buffer pointer.
- Concurrency/global state: follows active-interface dispatch state; callers handle surrounding locks.
- Portability/platform constraints: selected serial/I2C transport behavior remains behind adapter and platform hooks.
- Performance shape: preprocessor alias plus dispatcher and active callback cost.
- Security or parsing constraints: raw bytes are forwarded; no parsing.

## Direct Call Sites

Exact source search found:

- `n_lib.h:240` macro definition.
- `n_helpers.c:683` `NoteBinaryStoreTransmit` sends encoded binary data through `_ChunkedTransmit(encodedData, encLen + 1, false)`.

## Role In Callers

`NoteBinaryStoreTransmit` relies on this alias to send the binary payload after a successful setup transaction. It treats a non-null returned error as transfer failure, decodes the input buffer back to its original form, and returns the error.

## Tests Observed

`test/src/_noteChunkedTransmit_test.cpp` covers the expansion target's active-interface/no-interface behavior and parameter forwarding. `_noteSetActiveInterface_test.cpp` verifies serial/I2C callback registration and clearing. `NoteBinaryStoreTransmit_test.cpp` fakes `_noteChunkedTransmit` to verify caller failure handling through the alias target.

## Test Coverage Notes

Focused tests do not exercise macro expansion separately from the target function and do not run a full binary transmit through real serial/I2C adapters in one path.

## Graph Links

- Graph node id: `macro:n_lib.h:240:_ChunkedTransmit`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
