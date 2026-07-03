# `_jNew_Item`

## Status

Finalized. Source-inspected JSON node allocator/initializer.

## Identity

- Kind: function
- Definition: `n_cjson.c:194`
- Declaration/interface: file-local `NOTE_C_STATIC` forward at `n_cjson.c:89`
- Signature/API shape: `J *_jNew_Item(void)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:241:_Malloc` finalized
- External/system dependencies: `memset`
- Data/type/global dependencies: allocates through SDK allocator hook

## Behavior

Allocates `sizeof(J)` bytes through `_Malloc`. If allocation succeeds, zeroes the entire `J` structure with `memset`. Returns the allocated zeroed node or `NULL` on allocation failure.

## Important Boundaries

- Preconditions and assumptions: allocator hook must be installed for allocation to succeed.
- Error/null/empty behavior: allocation failure returns `NULL`.
- Ownership and lifetime: returns caller-owned `J` node; callers release it through `JDelete` or `_Free` paths.
- Concurrency/global state: allocation uses global allocator hook behind `_Malloc`.
- Portability/platform constraints: routes allocation through SDK hook.
- Performance shape: O(sizeof(J)).

## Direct Call Sites

Exact graph/source inspection found broad constructors and parser callers, including `JParseWithOpts`, `_parse_array`, `_parse_object`, `_create_reference`, typed JSON value creators, array/object creators, and `JDuplicate`.

## Role In Callers

In the active `JAddIntToObject` path, `JCreateInteger` uses `_jNew_Item` to allocate a zeroed JSON number node before assigning integer values.

## Tests Observed

None. No focused direct test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:194:_jNew_Item`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_Malloc`.

## Evidence Gaps

No unresolved source gaps for this node.
