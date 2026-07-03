# `_LockI2C`

## Status

Final. Macro expansion target, behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: macro
- Definition: `n_lib.h:245`
- Declaration/interface: internal macro in `n_lib.h:245`
- Public/exported names: none; target function `NoteLockI2C` is public
- Macro aliases/wrappers: `_LockI2C` expands to `NoteLockI2C`
- Signature/API shape: `_LockI2C()` macro with no arguments
- Visibility: internal transport macro
- Ownership/lifetime role: no ownership; aliases I2C lock hook invocation

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:616:NoteLockI2C` (expands_to, verified)
- External/system dependencies: platform lock hook through target function
- Data/type/global dependencies: target reads `hookLockI2C`
- Artifact coverage: generated artifact exists

## Behavior

Readability macro for `NoteLockI2C()`. It inherits target behavior: call registered I2C lock hook when set, no-op otherwise.

## Important Boundaries

- Preconditions and assumptions: caller expects I2C critical section semantics supplied by registered hook
- Error/null/empty behavior: no error channel; unset hook is no-op through `NoteLockI2C`
- Ownership and lifetime: none
- Concurrency/global state: inherits target global hook behavior
- Portability/platform constraints: platform behavior remains behind hook
- Performance shape: preprocessor alias to function call
- Security or parsing constraints: no parsing

## Direct Call Sites

Verified direct production callers: `n_i2c.c:92`, `n_i2c.c:200`, `n_i2c.c:340`, `n_i2c.c:458`, and `n_request.c:822`.

## Role In Callers

I2C lock primitive used by higher-level transaction/reset/chunk wrappers and ping-drain.

## Tests Observed

Focused evidence comes through `NoteLockI2C` tests and I2C transport tests that fake the macro target and assert balanced lock/unlock calls.

## Graph Links

- Graph node id: `macro:n_lib.h:245:_LockI2C`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this macro.
