# `_SerialReceive`

## Status

Final. Macro expansion target, behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: macro
- Definition: `n_lib.h:233`
- Declaration/interface: internal macro in `n_lib.h:233`
- Public/exported names: none; target helper `_noteSerialReceive` is internal
- Macro aliases/wrappers: `_SerialReceive` expands to `_noteSerialReceive`
- Signature/API shape: `_SerialReceive()` macro returning `char`
- Visibility: internal transport macro
- Ownership/lifetime role: no ownership; aliases serial receive dispatch helper

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:902:_noteSerialReceive` (expands_to, verified)
- External/system dependencies: platform serial receive hook through target helper
- Data/type/global dependencies: target reads `hookActiveInterface` and `hookSerialReceive`
- Artifact coverage: generated artifact exists

## Behavior

Readability macro for `_noteSerialReceive`. It inherits target behavior: active serial plus receive hook returns one hook byte; inactive or missing hook returns `'\0'`.

## Important Boundaries

- Preconditions and assumptions: callers generally check `_SerialAvailable()` before receiving
- Error/null/empty behavior: inactive or missing hook returns NUL character
- Ownership and lifetime: none
- Concurrency/global state: inherits target global hook/interface behavior
- Portability/platform constraints: platform receive behavior remains behind hook
- Performance shape: preprocessor alias to function call
- Security or parsing constraints: no parsing

## Direct Call Sites

Verified direct production callers: `n_serial.c:190`, `n_serial.c:263`, and `n_request.c:803`.

## Role In Callers

Serial receive primitive used by reset drain, chunked receive, and ping-drain paths.

## Tests Observed

Focused evidence comes through `_noteSerialReceive_test.cpp`; higher-level serial tests fake `_noteSerialReceive` for transaction/reset/chunk behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:233:_SerialReceive`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this macro.
