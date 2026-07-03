# `_noteI2CReset`

## Status

Finalized. Source-inspected I2C reset hook dispatcher, macro wrapper, hook state, and focused tests.

## Identity

- Kind: function
- Definition: `n_hooks.c:916`
- Declaration/interface: `n_lib.h:145`
- Public/exported names: none; internal hook dispatcher
- Macro aliases/wrappers: `_I2CReset` expands to `_noteI2CReset` at `n_lib.h:234`
- Signature/API shape: `bool _noteI2CReset(uint16_t DevAddress)`
- Visibility: internal transport hook boundary
- Ownership/lifetime role: no ownership; forwards address to configured platform hook

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: platform `i2cResetFn` hook when configured
- Data/type/global dependencies: reads `hookActiveInterface` and `hookI2CReset`
- Artifact coverage: implementation, declaration, macro wrapper, hook state setup/accessors, direct macro call sites, and focused tests inspected

## Behavior

If the active interface is I2C and `hookI2CReset` is non-null, calls `hookI2CReset(DevAddress)` and returns that bool. Otherwise returns `true` as a no-op success.

## Important Boundaries

- Preconditions and assumptions: address is already selected by callers; no validation is performed at this boundary.
- Error/null/empty behavior: inactive interface or missing reset hook is treated as successful reset. Only the platform hook can report failure.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: reads global active-interface and hook pointers without locking; transport wrappers that need mutual exclusion lock outside this helper.
- Portability/platform constraints: platform reset behavior is entirely behind the configured hook.
- Performance shape: O(1) plus hook cost.
- Security or parsing constraints: no parsing. `DevAddress` is `uint16_t`; I2C callers pass the `uint32_t` `NoteI2CAddress()` value through the `_I2CAddress` macro into this narrower parameter.

## Direct Call Sites

Exact source search found:

- `n_lib.h:234` macro wrapper `_I2CReset`.
- Direct production uses of that wrapper in `n_i2c.c:205`, `n_i2c.c:298`, and `n_i2c.c:494`.

Tests and fakes call `_noteI2CReset` directly, but production source uses the macro wrapper.

## Role In Callers

This is the platform-reset dispatch target behind all internal I2C reset operations. `_i2cNoteReset` treats a false reset as fatal on initial reset and on retry reset; `_i2cChunkedTransmit` invokes reset best-effort after transmit-hook errors and ignores the reset result while returning the original transmit error.

## Tests Observed

Focused `test/src/_noteI2CReset_test.cpp` covers inactive interface, null hook, active I2C hook dispatch, address forwarding, and return propagation. Hook setter/getter tests cover storage and retrieval of `hookI2CReset`; reset/transmit caller tests fake `_noteI2CReset` through the `_I2CReset` macro boundary.

## Graph Links

- Graph node id: `function:n_hooks.c:916:_noteI2CReset`
- Structured graph: `inventory/dependency-graph.json`
- Macro alias node: `macro:n_lib.h:234:_I2CReset`

## Evidence Gaps

No unresolved source gaps for this node.
