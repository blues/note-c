# `_DelayMs`

## Status

Finalized. Source-inspected delay macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:244`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_DelayMs`
- Visibility: internal macro
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:528:NoteDelayMs` finalized
- External/system dependencies: none
- Data/type/global dependencies: delay hook state through target

## Behavior

Expands directly to `NoteDelayMs`.

## Important Boundaries

- Preconditions and assumptions: delay value is forwarded unchanged.
- Error/null/empty behavior: target is a no-op if no delay hook is set.
- Ownership and lifetime: no allocation.
- Concurrency/global state: follows delay hook state.
- Performance shape: O(1) plus hook delay.
- Portability/platform constraints: platform delay remains behind hook target.

## Direct Call Sites

Exact source search found retry paths and adapter code call `_DelayMs`.

## Role In Callers

`_noteTransactionShouldLock` uses this alias for retry backoff.

## Tests Observed

`NoteDelayMs_test.cpp` and transaction retry tests cover delay behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:244:_DelayMs`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
