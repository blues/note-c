# `NoteMalloc`

## Status

Finalized. Source-inspected platform-hook allocation wrapper.

## Identity

- Kind: function
- Definition: `n_hooks.c:569`
- Declaration/interface: `note.h:1051`
- Signature/API shape: `void *NoteMalloc(size_t size)`
- Visibility: public API declaration, implemented in `n_hooks.c`
- Public/exported names: `NoteMalloc`
- Macro aliases/wrappers: `_Malloc` in `n_lib.h:241`

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:308:_noteIsDebugOutputActive` finalized
  - `function:n_cjson_helpers.c:284:JItoA` finalized
  - `function:n_hooks.c:561:_n_ptoa32` finalized
- External/system dependencies: platform hook `hookMalloc`
- Data/type/global dependencies: reads static `hookMalloc`; optionally reads `hookDebugOutput` through debug predicate

## Behavior

Returns `NULL` immediately if no allocation hook has been registered in `hookMalloc`. Otherwise calls `hookMalloc(size)` and returns that pointer. When `NOTE_C_SHOW_MALLOC && !NOTE_C_LOW_MEM`, and debug output is active, it writes a `"malloc "` debug line containing the requested size, either `" FAIL"` for a null allocation result, or the allocated pointer rendered through `_n_ptoa32`.

## Important Boundaries

- Preconditions and assumptions: allocation behavior is entirely supplied by the platform hook installed via hook setup APIs.
- Error/null/empty behavior: no hook or hook allocation failure returns `NULL`; no error string is produced.
- Ownership and lifetime: transfers allocated memory ownership to the caller according to the registered hook's contract.
- Concurrency/global state: reads hook globals without taking the Notecard lock.
- Portability/platform constraints: allocation hook keeps memory policy outside the portable SDK core; debug pointer printing is excluded in low-memory builds and constrained to 32-bit pointers by `_n_ptoa32`.
- Performance shape: one hook call plus optional debug formatting.

## Direct Call Sites

Exact source search found no production direct calls to `NoteMalloc` except the `_Malloc` alias and the implementation/declaration. `_Malloc` call-site fanout includes JSON allocation, request serialization buffers, serial/I2C transaction buffers, and payload helpers.

## Role In Callers

`_Malloc` aliases to `NoteMalloc` so internal source can allocate through the SDK's registered allocator hook without depending on libc allocation directly.

## Tests Observed

None. No focused direct `NoteMalloc` test was inspected.

## Graph Links

- Graph node id: `function:n_hooks.c:569:NoteMalloc`
- Structured graph: `inventory/dependency-graph.json`
- Verified alias edge from `macro:n_lib.h:241:_Malloc`.

## Evidence Gaps

No unresolved source gaps for this node.
