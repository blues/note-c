# `_ensure`

## Status

Finalized. Source-inspected JSON print buffer growth helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:351`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `unsigned char *_ensure(printbuffer * const p, size_t needed)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:241:_Malloc` finalized
  - `macro:n_lib.h:242:_Free` finalized
- External/system dependencies: `memcpy`
- Data/type/global dependencies: mutates caller-owned `printbuffer`

## Behavior

Ensures a print buffer has room for `needed` additional bytes beyond the current offset plus a terminator. It rejects null buffers, null backing storage, invalid offsets, sizes above `INT_MAX`, and no-allocation buffers that need growth. If current capacity is sufficient, it returns `p->buffer + p->offset`. Otherwise it allocates a new `ALLOC_CHUNK`-rounded buffer, copies bytes through `p->offset + 1`, frees the old backing buffer, updates `p->length` and `p->buffer`, and returns the new write pointer. If allocation fails, it frees the old backing buffer, zeroes `p->length`, clears `p->buffer`, and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `p->buffer` must point to allocation compatible with `_Free`.
- Error/null/empty behavior: invalid inputs, unsupported huge sizes, noalloc growth, or allocation failure return `NULL`.
- Ownership and lifetime: may replace and free the buffer owned by the printbuffer.
- Concurrency/global state: no global state; allocation/free use hook-backed aliases.
- Performance shape: O(existing offset) when growth occurs; O(1) when capacity is sufficient.

## Direct Call Sites

Exact source search found callers in `_print_number`, `_print_string_ptr`, `_print_value`, `_print_array`, `_print_object`, and buffered/preallocated printing paths.

## Role In Callers

JSON printing helpers use `_ensure` as their dynamic print-buffer growth and bounds-checking primitive.

## Tests Observed

Focused `JPrintUnformatted` and JSON number printing tests exercise this helper through public printing APIs. No direct unit test for `_ensure` was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:351:_ensure`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_Malloc` and `_Free`.

## Evidence Gaps

No unresolved source gaps for this node.
