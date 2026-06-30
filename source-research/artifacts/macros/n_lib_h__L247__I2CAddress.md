# `_I2CAddress`

## Status

Final. Macro expansion target, behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: macro
- Definition: `n_lib.h:247`
- Declaration/interface: internal macro in `n_lib.h:247`
- Public/exported names: none; target function `NoteI2CAddress` is public
- Macro aliases/wrappers: `_I2CAddress` expands to `NoteI2CAddress`
- Signature/API shape: `_I2CAddress()` macro with no arguments returning `uint32_t`
- Visibility: internal transport macro
- Ownership/lifetime role: no ownership; aliases effective I2C address accessor

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:969:NoteI2CAddress` (expands_to, verified)
- External/system dependencies: none
- Data/type/global dependencies: target reads `i2cAddress`
- Artifact coverage: generated artifact exists

## Behavior

Readability macro for `NoteI2CAddress()`. It returns the target's effective I2C address, using the default address when stored address is zero.

## Important Boundaries

- Preconditions and assumptions: address state is configured by I2C setters
- Error/null/empty behavior: zero address maps to default through target function
- Ownership and lifetime: none
- Concurrency/global state: inherits target global state behavior
- Portability/platform constraints: default address is SDK constant
- Performance shape: preprocessor alias to function call
- Security or parsing constraints: no parsing

## Direct Call Sites

Verified direct production callers in `n_i2c.c`: query, transaction reset/transmit/receive/reset retry/chunk transmit paths; and in `n_request.c` ping-drain receive calls.

## Role In Callers

Effective address primitive for all I2C platform hook calls.

## Tests Observed

Focused evidence comes through `NoteI2CAddress_test.cpp` and I2C reset tests that assert hook calls receive `NoteI2CAddress()`.

## Graph Links

- Graph node id: `macro:n_lib.h:247:_I2CAddress`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this macro.
