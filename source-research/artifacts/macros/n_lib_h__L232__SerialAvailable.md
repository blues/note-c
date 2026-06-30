# `_SerialAvailable`

## Status

Final. Macro expansion target, behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: macro
- Definition: `n_lib.h:232`
- Declaration/interface: internal macro in `n_lib.h:232`
- Public/exported names: none; target helper `_noteSerialAvailable` is internal
- Macro aliases/wrappers: `_SerialAvailable` expands to `_noteSerialAvailable`
- Signature/API shape: `_SerialAvailable()` macro returning `bool`
- Visibility: internal transport macro
- Ownership/lifetime role: no ownership; aliases serial availability dispatch helper

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:887:_noteSerialAvailable` (expands_to, verified)
- External/system dependencies: platform serial availability hook through target helper
- Data/type/global dependencies: target reads `hookActiveInterface` and `hookSerialAvailable`
- Artifact coverage: generated artifact exists

## Behavior

Readability macro for `_noteSerialAvailable`. It inherits target behavior: active serial plus availability hook returns hook result; inactive or missing hook returns `false`.

## Important Boundaries

- Preconditions and assumptions: serial interface and availability hook should be configured before callers depend on true availability
- Error/null/empty behavior: inactive or missing hook returns `false`
- Ownership and lifetime: none
- Concurrency/global state: inherits target global hook/interface behavior
- Portability/platform constraints: platform availability behavior remains behind hook
- Performance shape: preprocessor alias to function call
- Security or parsing constraints: no parsing

## Direct Call Sites

Verified direct production callers: `n_serial.c:70`, `n_serial.c:185`, `n_serial.c:243`, and `n_request.c:802`.

## Role In Callers

Serial availability primitive used by serial response wait, reset drain, chunked receive, and ping-drain paths.

## Tests Observed

Focused evidence comes through `_noteSerialAvailable_test.cpp`; higher-level serial tests fake `_noteSerialAvailable` for transaction/reset/chunk behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:232:_SerialAvailable`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this macro.
