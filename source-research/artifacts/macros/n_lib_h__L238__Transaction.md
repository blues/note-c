# `_Transaction`

## Status

Finalized. Source-inspected JSON transaction macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:238`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_Transaction`
- Visibility: internal macro
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:1042:_noteJSONTransaction` finalized
- External/system dependencies: none
- Data/type/global dependencies: active interface dispatch state through target

## Behavior

Expands directly to `_noteJSONTransaction`.

## Important Boundaries

- Preconditions and assumptions: request/length/response/timeout are forwarded to the active adapter target.
- Error/null/empty behavior: target returns an error string if no valid interface is selected.
- Ownership and lifetime: response ownership follows the selected adapter target.
- Concurrency/global state: follows active-interface dispatch state.
- Performance shape: O(1) plus adapter transaction cost.
- Portability/platform constraints: serial/I2C behavior remains behind hook target.

## Direct Call Sites

Exact source search found request, raw JSON, and ping paths call `_Transaction`.

## Role In Callers

`_noteTransactionShouldLock` uses this alias for command and request I/O inside the retry loop.

## Tests Observed

JSON transaction, serial/I2C adapter, and transaction retry tests cover this alias.

## Graph Links

- Graph node id: `macro:n_lib.h:238:_Transaction`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
