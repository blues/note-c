# `_I2CReset`

## Status

Finalized. Source-inspected I2C reset macro alias, target behavior, direct call sites, and focused tests.

## Identity

- Kind: macro
- Definition: `n_lib.h:234`
- Declaration/interface: internal macro in `n_lib.h:234`
- Public/exported names: none; target helper `_noteI2CReset` is internal
- Macro aliases/wrappers: `_I2CReset` expands to `_noteI2CReset`
- Signature/API shape: `_I2CReset(address)` returning `bool`
- Visibility: internal transport macro
- Ownership/lifetime role: no ownership; aliases I2C reset hook dispatcher

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:916:_noteI2CReset` finalized in this slice
- External/system dependencies: platform I2C reset hook through target helper
- Data/type/global dependencies: target reads `hookActiveInterface` and `hookI2CReset`
- Artifact coverage: macro definition, target helper, direct production callers, and focused tests inspected

## Behavior

Readability macro for `_noteI2CReset`. It inherits target behavior: when I2C is active and a reset hook is configured, call that hook with the supplied address and return its boolean result; otherwise return true as no-op success.

## Important Boundaries

- Preconditions and assumptions: caller supplies effective I2C address, usually through `_I2CAddress()`.
- Error/null/empty behavior: missing hook/inactive interface is success through the target helper.
- Ownership and lifetime: none.
- Concurrency/global state: no lock in the macro or target; callers own locking.
- Portability/platform constraints: platform reset behavior remains behind the configured hook.
- Performance shape: preprocessor alias to O(1) dispatcher plus hook cost.
- Security or parsing constraints: no parsing; address narrowing follows target's `uint16_t` parameter.

## Direct Call Sites

Exact production callers:

- `n_i2c.c:205` `_i2cNoteReset` performs the initial bus reset while holding the I2C lock; false return logs, unlocks, and fails reset.
- `n_i2c.c:298` `_i2cNoteReset` retries a bus reset after a reset-sync attempt finds no response; false return logs and exits the retry loop.
- `n_i2c.c:494` `_i2cChunkedTransmit` invokes a best-effort reset after `_I2CTransmit` returns an error; the reset result is ignored and the original transmit error is returned.

## Role In Callers

Shared internal reset primitive for I2C reset and transmit-error recovery paths. Its no-op success behavior lets platforms omit a reset hook, while callers that do receive false decide whether failure is fatal.

## Tests Observed

Focused evidence includes `_noteI2CReset_test.cpp` for target dispatch, `_i2cNoteReset_test.cpp` for reset-call address/locking/retry behavior, `_i2cChunkedTransmit_test.cpp` for transmit-error reset invocation, and `_noteSetActiveInterface_test.cpp` for I2C reset dispatch wiring.

## Graph Links

- Graph node id: `macro:n_lib.h:234:_I2CReset`
- Structured graph: `inventory/dependency-graph.json`
- Verified expands-to target: `_noteI2CReset`

## Evidence Gaps

No unresolved source gaps for this node. No focused test was observed for `_i2cChunkedTransmit` behavior when the best-effort reset itself returns false; source ignores that result.
