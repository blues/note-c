# `_noteI2CTransmit`

## Status

Finalized. Source-inspected I2C transmit hook dispatcher, macro wrapper, hook state, and focused tests.

## Identity

- Kind: function
- Definition: `n_hooks.c:934`
- Declaration/interface: `n_lib.h:146`
- Public/exported names: none; internal hook dispatcher
- Macro aliases/wrappers: `_I2CTransmit` expands to `_noteI2CTransmit` at `n_lib.h:235`
- Signature/API shape: `const char *_noteI2CTransmit(uint16_t DevAddress, const uint8_t *pBuffer, uint16_t Size)`
- Visibility: internal transport hook boundary
- Ownership/lifetime role: forwards caller buffer to configured platform hook; does not allocate or own data

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: platform `i2cTransmitFn` hook when configured
- Data/type/global dependencies: reads `hookActiveInterface` and `hookI2CTransmit`
- Artifact coverage: implementation, declaration, macro wrapper, hook state setup/accessors, direct macro call sites, and focused tests inspected

## Behavior

If the active interface is I2C and `hookI2CTransmit` is non-null, forwards the address, buffer, and size to the platform hook and returns its error string. Otherwise returns the literal `"i2c not active"`.

The dispatcher intentionally casts the `const uint8_t *` buffer to `uint8_t *` because the public hook typedef is still mutable for API compatibility.

## Important Boundaries

- Preconditions and assumptions: active I2C interface and transmit hook must be configured for a real transmit; the hook must honor `Size`.
- Error/null/empty behavior: inactive interface or missing hook returns `"i2c not active"`; platform hook controls all other error strings.
- Ownership and lifetime: no allocation or ownership transfer; passes the caller buffer pointer through to the hook.
- Concurrency/global state: reads global active-interface and hook pointers without locking; transport wrappers that need mutual exclusion lock outside this helper.
- Portability/platform constraints: platform transmit behavior is entirely behind the configured hook.
- Performance shape: O(1) plus hook cost.
- Security or parsing constraints: no parsing; constness is cast away to satisfy `i2cTransmitFn`.

## Direct Call Sites

Exact source search found:

- `n_lib.h:235` macro wrapper `_I2CTransmit`.
- Direct production uses of that wrapper in `n_i2c.c:223` and `n_i2c.c:492`.

Tests and fakes call `_noteI2CTransmit` directly, but production source uses the macro wrapper.

## Role In Callers

This is the platform transmit dispatch target behind I2C reset resync and chunked transmit paths. Callers interpret any non-null return as a transmit error.

## Tests Observed

Focused `test/src/_noteI2CTransmit_test.cpp` covers inactive interface, null hook, active I2C hook dispatch, parameter forwarding, and hook return propagation. Hook setter/getter tests cover storage and retrieval of `hookI2CTransmit`; reset/chunk caller tests fake `_noteI2CTransmit` through the `_I2CTransmit` macro boundary.

## Graph Links

- Graph node id: `function:n_hooks.c:934:_noteI2CTransmit`
- Structured graph: `inventory/dependency-graph.json`
- Macro alias node: `macro:n_lib.h:235:_I2CTransmit`

## Evidence Gaps

No unresolved source gaps for this node.
