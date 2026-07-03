# `_noteJSONTransaction`

## Status

Finalized. Source-inspected active-interface JSON transaction dispatcher.

## Identity

- Kind: function
- Definition: `n_hooks.c:1042`
- Declaration/interface: internal hook target
- Signature/API shape: `const char *_noteJSONTransaction(const char *request, size_t reqLen, char **response, uint32_t timeoutMs)`
- Visibility: internal note-c hook boundary
- Public/exported names: `_noteJSONTransaction`

## Dependency Status

- Project-local dependencies: selected transaction target is dynamic through `notecardTransaction`
- External/system dependencies: none
- Data/type/global dependencies: `notecardTransaction`, `hookActiveInterface`

## Behavior

Returns the error string `"a valid interface must be selected"` when no transaction target is selected or the active interface is none. Otherwise forwards request pointer, request length, response pointer, and timeout unchanged to the selected transaction function pointer and returns its result.

## Important Boundaries

- Preconditions and assumptions: caller supplies newline-terminated request text according to transport contract.
- Error/null/empty behavior: disabled interface returns an error string rather than attempting I/O.
- Ownership and lifetime: response ownership is determined by the selected adapter.
- Concurrency/global state: reads active interface and transaction dispatch pointer.
- Performance shape: O(1) plus adapter transaction cost.
- Portability/platform constraints: serial/I2C behavior remains behind adapter targets selected by `_noteSetActiveInterface`.

## Direct Call Sites

Exact source search found macro alias `_Transaction` as the primary internal caller surface.

## Role In Callers

`_noteTransactionShouldLock`, raw JSON request flow, and ping flow use this dispatcher for Notecard I/O.

## Tests Observed

`_noteJSONTransaction_test.cpp`, `_noteSetActiveInterface_test.cpp`, serial/I2C transaction tests, and transaction tests cover disabled-interface and forwarding behavior.

## Graph Links

- Graph node id: `function:n_hooks.c:1042:_noteJSONTransaction`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
