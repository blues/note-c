# `_Reset`

## Status

Finalized. Source-inspected reset macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:237`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_Reset`
- Visibility: internal macro
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:1015:_noteHardReset` finalized
- External/system dependencies: none
- Data/type/global dependencies: active interface dispatch state through target

## Behavior

Expands directly to `_noteHardReset`.

## Important Boundaries

- Preconditions and assumptions: active reset target is selected elsewhere.
- Error/null/empty behavior: target returns true if no reset hook is selected.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows active-interface dispatch state.
- Performance shape: O(1) plus adapter reset cost.
- Portability/platform constraints: adapter-specific reset remains behind hook target.

## Direct Call Sites

Exact source search found transaction and public reset paths call `_Reset`.

## Role In Callers

`_noteTransactionShouldLock` uses this alias before I/O when reset is required and during retry after I/O errors.

## Tests Observed

Reset and transaction retry tests cover this alias.

## Graph Links

- Graph node id: `macro:n_lib.h:237:_Reset`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
