# `_noteI2CReceive`

## Status

Final. Behavior, internal declaration, hook dispatch, error/null behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:955`
- Declaration/interface: `n_lib.h:147`
- Public/exported names: internal `_noteI2CReceive`; public hook setters are `NoteSetFnI2C` and `NoteSetFnI2CDefault`
- Macro aliases/wrappers: `_I2CReceive` expands to `_noteI2CReceive` at `n_lib.h:236`
- Signature/API shape: `const char *_noteI2CReceive(uint16_t DevAddress, uint8_t *pBuffer, uint16_t Size, uint32_t *available)`
- Visibility: internal I2C receive dispatch helper
- Ownership/lifetime role: forwards caller-owned buffer and availability pointer to platform hook; no ownership

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: platform I2C receive hook
- Data/type/global dependencies: reads `hookActiveInterface` and `hookI2CReceive`
- Artifact coverage: generated artifact exists

## Behavior

If `hookActiveInterface == NOTE_C_INTERFACE_I2C` and `hookI2CReceive != NULL`, forwards all arguments to `hookI2CReceive` and returns that hook's result. Otherwise returns the static error string `"i2c not active"`.

## Important Boundaries

- Preconditions and assumptions: active interface and receive hook must be configured before I2C transport receive paths use this helper
- Error/null/empty behavior: inactive interface or missing hook returns `"i2c not active"`; active path performs no local null checks on `pBuffer` or `available`
- Ownership and lifetime: does not allocate or retain pointers
- Concurrency/global state: reads global active-interface and hook state without local locking
- Portability/platform constraints: platform receive behavior stays behind hook
- Performance shape: constant-time gate plus platform hook call
- Security or parsing constraints: no parsing; trusts hook to honor buffer size and availability pointer

## Direct Call Sites

Direct internal macro callers use `_I2CReceive` in `n_i2c.c:55`, `n_i2c.c:253`, `n_i2c.c:382`, `n_request.c:823`, and `n_request.c:831`. Focused direct tests are in `test/src/_noteI2CReceive_test.cpp`.

## Role In Callers

I2C receive dispatch primitive for query, reset/drain, chunk receive, and ping-drain paths.

## Tests Observed

`test/src/_noteI2CReceive_test.cpp` verifies inactive interface and missing receive hook both return `"i2c not active"`, and active I2C plus a receive hook forwards address, buffer, size, and available pointer while returning the hook result.

## Graph Links

- Graph node id: `function:n_hooks.c:955:_noteI2CReceive`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers active-path null pointer misuse; source shows parameters are passed directly to the hook.
