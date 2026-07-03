# `_ChunkedReceive`

## Status

Finalized. Source-inspected chunked receive macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:239`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_ChunkedReceive`
- Visibility: internal macro
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:1068:_noteChunkedReceive` finalized
- External/system dependencies: none
- Data/type/global dependencies: active interface dispatch state through target

## Behavior

Expands directly to `_noteChunkedReceive`.

## Important Boundaries

- Preconditions and assumptions: buffer/size/delay/timeout/available arguments are forwarded to the active adapter target.
- Error/null/empty behavior: target returns an error string if no valid interface is selected.
- Ownership and lifetime: no allocation in alias or dispatcher.
- Concurrency/global state: follows active-interface dispatch state.
- Performance shape: O(1) plus adapter receive cost.
- Portability/platform constraints: serial/I2C behavior remains behind hook target.

## Direct Call Sites

Exact source search found `NoteBinaryStoreReceive` as the active slice caller.

## Role In Callers

`NoteBinaryStoreReceive` uses this alias to receive the raw binary payload after validating the Notecard response fields.

## Tests Observed

`NoteBinaryStoreReceive_test.cpp` and `_noteChunkedReceive_test.cpp` cover receive error and forwarding behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:239:_ChunkedReceive`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
