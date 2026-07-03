# `_update_offset`

## Status

Finalized. Source-inspected JSON print offset updater.

## Identity

- Kind: function
- Definition: `n_cjson.c:400`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `void _update_offset(printbuffer * const buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `strlen`
- Data/type/global dependencies: mutates `buffer->offset`

## Behavior

If the printbuffer or backing buffer is null, returns without changing state. Otherwise computes `buffer_pointer = buffer->buffer + buffer->offset` and advances `buffer->offset` by the NUL-terminated string length starting at that pointer.

## Important Boundaries

- Preconditions and assumptions: caller has written a NUL-terminated fragment at the current offset.
- Error/null/empty behavior: null printbuffer or backing buffer is a no-op.
- Ownership and lifetime: no allocation or free.
- Concurrency/global state: no global state.
- Performance shape: O(length of just-written fragment).

## Direct Call Sites

Exact source search found callers in `_print`, `_print_array`, and `_print_object`.

## Role In Callers

Printing helpers call `_update_offset` after nested value/key writes where the nested helper writes a NUL-terminated fragment but does not directly add a known byte count to the caller's offset.

## Tests Observed

Focused public printing tests exercise this indirectly. No direct unit test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:400:_update_offset`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
