# `_noteChunkedReceive`

## Status

Finalized. Source-inspected active-interface chunked receive dispatcher.

## Identity

- Kind: function
- Definition: `n_hooks.c:1068`
- Declaration/interface: `n_lib.h:150`
- Signature/API shape: `const char *_noteChunkedReceive(uint8_t *buffer, uint32_t *size, bool delay, uint32_t timeoutMs, uint32_t *available)`
- Visibility: internal note-c hook boundary
- Public/exported names: `_noteChunkedReceive`

## Dependency Status

- Project-local dependencies: selected chunked receive target is dynamic through `notecardChunkedReceive`
- External/system dependencies: none
- Data/type/global dependencies: `notecardChunkedReceive`, `hookActiveInterface`

## Behavior

Returns `"a valid interface must be selected"` when no chunked receive target is selected or the active interface is none. Otherwise forwards buffer, size pointer, delay flag, timeout, and available pointer to the selected chunked receive function pointer and returns its error string.

## Important Boundaries

- Preconditions and assumptions: caller supplies buffers and in/out pointers according to active adapter contract.
- Error/null/empty behavior: disabled interface returns an error string.
- Ownership and lifetime: no allocation in dispatcher; adapter owns any transport-specific behavior.
- Concurrency/global state: reads active interface and dispatch pointer.
- Performance shape: O(1) plus adapter receive cost.
- Portability/platform constraints: serial/I2C behavior remains behind adapter targets selected by `_noteSetActiveInterface`.

## Direct Call Sites

Exact source search found macro alias `_ChunkedReceive` as the primary internal caller surface.

## Role In Callers

`NoteBinaryStoreReceive` uses this dispatcher to read raw binary chunks after the JSON setup transaction succeeds.

## Tests Observed

`_noteChunkedReceive_test.cpp`, `_noteSetActiveInterface_test.cpp`, and `NoteBinaryStoreReceive_test.cpp` cover disabled-interface and forwarding behavior.

## Graph Links

- Graph node id: `function:n_hooks.c:1068:_noteChunkedReceive`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
