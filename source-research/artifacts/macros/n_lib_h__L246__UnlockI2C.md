# `_UnlockI2C`

## Status

Final. Macro expansion target, behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: macro
- Definition: `n_lib.h:246`
- Declaration/interface: internal macro in `n_lib.h:246`
- Public/exported names: none; target function `NoteUnlockI2C` is public
- Macro aliases/wrappers: `_UnlockI2C` expands to `NoteUnlockI2C`
- Signature/API shape: `_UnlockI2C()` macro with no arguments
- Visibility: internal transport macro
- Ownership/lifetime role: no ownership; aliases I2C unlock hook invocation

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:628:NoteUnlockI2C` (expands_to, verified)
- External/system dependencies: platform unlock hook through target function
- Data/type/global dependencies: target reads `hookUnlockI2C`
- Artifact coverage: generated artifact exists

## Behavior

Readability macro for `NoteUnlockI2C()`. It inherits target behavior: call registered I2C unlock hook when set, no-op otherwise.

## Important Boundaries

- Preconditions and assumptions: caller expects it to balance a preceding `_LockI2C()` on paths that took the lock
- Error/null/empty behavior: no error channel; unset hook is no-op through `NoteUnlockI2C`
- Ownership and lifetime: none
- Concurrency/global state: inherits target global hook behavior
- Portability/platform constraints: platform behavior remains behind hook
- Performance shape: preprocessor alias to function call
- Security or parsing constraints: no parsing

## Direct Call Sites

Verified direct production callers are I2C unlock paths in `n_i2c.c` and ping drain in `n_request.c:824`, `n_request.c:832`, `n_request.c:836`.

## Role In Callers

I2C unlock primitive used by higher-level transaction/reset/chunk wrappers and ping-drain.

## Tests Observed

Focused evidence comes through `NoteUnlockI2C` tests and I2C transport tests that fake the macro target and assert balanced lock/unlock calls.

## Graph Links

- Graph node id: `macro:n_lib.h:246:_UnlockI2C`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this macro.
