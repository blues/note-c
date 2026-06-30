# `NoteGetActiveInterface`

## Status

Finalized. Source-inspected active-interface getter.

## Identity

- Kind: function
- Definition: `n_hooks.c:209`
- Declaration/interface: `note.h:783`
- Signature/API shape: `int NoteGetActiveInterface(void)`
- Visibility: public note-c hook API
- Public/exported names: `NoteGetActiveInterface`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `hookActiveInterface`

## Behavior

Returns the current `hookActiveInterface` value, one of the note-c interface constants or `NOTE_C_INTERFACE_NONE`.

## Important Boundaries

- Preconditions and assumptions: no inputs.
- Error/null/empty behavior: no error channel.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads global hook state.
- Performance shape: O(1).
- Portability/platform constraints: none.

## Direct Call Sites

Exact source search found callers in `NoteUserAgent`, `NoteReset`, and tests.

## Role In Callers

`NoteUserAgent` uses this value to record `"req_interface"` as `none`, `serial`, `i2c`, or `unknown`.

## Tests Observed

`NoteGetActiveInterface_test.cpp` covers the getter. Interface-setting tests exercise it indirectly.

## Graph Links

- Graph node id: `function:n_hooks.c:209:NoteGetActiveInterface`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
