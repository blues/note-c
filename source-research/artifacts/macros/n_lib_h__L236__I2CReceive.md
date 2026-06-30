# `_I2CReceive`

## Status

Final. Macro expansion target, behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: macro
- Definition: `n_lib.h:236`
- Declaration/interface: internal macro in `n_lib.h:236`
- Public/exported names: none; target helper `_noteI2CReceive` is internal
- Macro aliases/wrappers: `_I2CReceive` expands to `_noteI2CReceive`
- Signature/API shape: `_I2CReceive(addr, buffer, size, available)` returning `const char *`
- Visibility: internal transport macro
- Ownership/lifetime role: no ownership; aliases I2C receive dispatch helper

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:955:_noteI2CReceive` (expands_to, verified)
- External/system dependencies: platform receive hook through target helper
- Data/type/global dependencies: target reads `hookActiveInterface` and `hookI2CReceive`
- Artifact coverage: generated artifact exists

## Behavior

Readability macro for `_noteI2CReceive`. It inherits target behavior: active I2C plus receive hook forwards all parameters to the platform hook; otherwise returns `"i2c not active"`.

## Important Boundaries

- Preconditions and assumptions: I2C interface and receive hook should be configured before callers depend on successful receive
- Error/null/empty behavior: inactive/missing hook returns `"i2c not active"`; active path passes pointers directly to hook
- Ownership and lifetime: none
- Concurrency/global state: inherits target global hook/interface behavior
- Portability/platform constraints: platform receive behavior remains behind hook
- Performance shape: preprocessor alias to function call
- Security or parsing constraints: no parsing; buffer safety depends on callers and hook honoring size

## Direct Call Sites

Verified direct production callers: `n_i2c.c:55`, `n_i2c.c:253`, `n_i2c.c:382`, `n_request.c:823`, and `n_request.c:831`.

## Role In Callers

I2C receive primitive used by query, reset/drain, chunk receive, and ping-drain paths.

## Tests Observed

Focused evidence comes through `_noteI2CReceive_test.cpp`; higher-level I2C tests fake `_noteI2CReceive` and assert receive sizing/address behavior in query/reset/chunk paths.

## Graph Links

- Graph node id: `macro:n_lib.h:236:_I2CReceive`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this macro.
