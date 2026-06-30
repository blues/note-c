# `_delayIO`

## Status

Finalized. Source-inspected I2C pacing helper.

## Identity

- Kind: function
- Definition: `n_i2c.c:30`
- Declaration/interface: file-local forward at `n_i2c.c:19`; test-only exposure in `test/include/test_static.h:21`
- Signature/API shape: `void _delayIO(void)`
- Visibility: internal to `n_i2c.c` unless statics are exposed for tests
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:244:_DelayMs` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads global `cardTurboIO` declared in `n_lib.h:170` and defined in `n_helpers.c:61`
- Artifact coverage: source, direct callers, and focused tests inspected

## Behavior

If `cardTurboIO` is false, calls `_DelayMs(6)`. If `cardTurboIO` is true, returns without delay.

## Important Boundaries

- Preconditions and assumptions: none beyond initialized global hook state for `_DelayMs`.
- Error/null/empty behavior: no return value or error channel; delay hook absence is handled by `NoteDelayMs`.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads global `cardTurboIO` without locking.
- Portability/platform constraints: actual delay remains behind the SDK delay hook.
- Performance shape: O(1) plus optional platform delay.
- Security or parsing constraints: none.

## Direct Call Sites

Exact production callers:

- `n_i2c.c:111` `_i2cNoteTransaction` delays after transmit before querying response length.
- `n_i2c.c:211` `_i2cNoteReset` delays after reset.
- `n_i2c.c:302` `_i2cNoteReset` delays between reset retry attempts.
- `n_i2c.c:490` `_i2cChunkedTransmit` delays before chunk transmit.

## Role In Callers

Provides conservative I2C pacing for transport operations unless turbo I/O mode disables that pacing.

## Tests Observed

`test/src/_i2cNoteReset_test.cpp` fakes `_delayIO` and checks reset paths that invoke it. No dedicated direct `_delayIO` behavior test was found.

## Graph Links

- Graph node id: `function:n_i2c.c:30:_delayIO`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_DelayMs`

## Evidence Gaps

No unresolved source gaps for this node. Direct focused tests for `cardTurboIO` true/false behavior were not found.
