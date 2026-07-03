# `_I2CTransmit`

## Status

Finalized. Source-inspected I2C transmit macro alias, target behavior, direct call sites, and focused tests.

## Identity

- Kind: macro
- Definition: `n_lib.h:235`
- Declaration/interface: internal macro in `n_lib.h:235`
- Public/exported names: none; target helper `_noteI2CTransmit` is internal
- Macro aliases/wrappers: `_I2CTransmit` expands to `_noteI2CTransmit`
- Signature/API shape: `_I2CTransmit(address, buffer, size)` returning `const char *`
- Visibility: internal transport macro
- Ownership/lifetime role: no ownership; aliases I2C transmit hook dispatcher

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:934:_noteI2CTransmit` finalized in this slice
- External/system dependencies: platform I2C transmit hook through target helper
- Data/type/global dependencies: target reads `hookActiveInterface` and `hookI2CTransmit`
- Artifact coverage: macro definition, target helper, direct production callers, and focused tests inspected

## Behavior

Readability macro for `_noteI2CTransmit`. It inherits target behavior: active I2C with a transmit hook forwards address, buffer, and size to the hook; otherwise returns `"i2c not active"`.

## Important Boundaries

- Preconditions and assumptions: caller supplies effective I2C address, buffer pointer, and size that fits the target `uint16_t`.
- Error/null/empty behavior: inactive interface or missing hook returns an error string through the target; hook errors are returned unchanged.
- Ownership and lifetime: none; caller owns the buffer.
- Concurrency/global state: no lock in the macro or target; callers own locking.
- Portability/platform constraints: platform transmit behavior remains behind the configured hook.
- Performance shape: preprocessor alias to O(1) dispatcher plus hook cost.
- Security or parsing constraints: no parsing; target casts away `const` for legacy hook typedef compatibility.

## Direct Call Sites

Exact production callers:

- `n_i2c.c:223` `_i2cNoteReset` sends a stack newline byte during reset/resync while holding the I2C lock.
- `n_i2c.c:492` `_i2cChunkedTransmit` sends each capped transmit chunk.

## Role In Callers

Shared internal I2C transmit primitive for reset resynchronization and chunked request/binary transmit paths.

## Tests Observed

Focused evidence includes `_noteI2CTransmit_test.cpp` for target dispatch, `_i2cNoteReset_test.cpp` for reset newline transmit behavior, `_i2cChunkedTransmit_test.cpp` for chunk transmit behavior, and `_i2cNoteTransaction_test.cpp` for request transmit error propagation.

## Graph Links

- Graph node id: `macro:n_lib.h:235:_I2CTransmit`
- Structured graph: `inventory/dependency-graph.json`
- Verified expands-to target: `_noteI2CTransmit`

## Evidence Gaps

No unresolved source gaps for this node.
