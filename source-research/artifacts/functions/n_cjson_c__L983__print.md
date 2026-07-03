# `_print`

## Status

Finalized. Source-inspected allocating JSON print entry.

## Identity

- Kind: function
- Definition: `n_cjson.c:983`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `unsigned char *_print(const J * const item, Jbool format, Jbool omitempty)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_cjson.c:1179:_print_value` finalized
  - `function:n_cjson.c:400:_update_offset` finalized
  - `macro:n_cjson.c:981:cjson_min` finalized
  - `macro:n_lib.h:242:_Free` finalized
- External/system dependencies: `memset`, `memcpy`
- Data/type/global dependencies: `printbuffer` fields, default initial buffer size `128`

## Behavior

Initializes a stack `printbuffer`, allocates a default 128-byte working buffer, records the requested formatted and omit-empty modes, and delegates value serialization to `_print_value`. After updating the offset, it allocates an exact-size result buffer of `offset + 1`, copies the bounded working-buffer content with `cjson_min(buffer->length, buffer->offset + 1)`, forcibly NUL-terminates the result, frees the working buffer, and returns the caller-owned result. Any allocation or print failure frees owned buffers and returns NULL.

## Important Boundaries

- Preconditions and assumptions: public wrappers perform null checks before calling; `_print` itself does not reject null `item` before `_print_value`.
- Error/null/empty behavior: allocation failure or `_print_value` failure returns NULL after cleanup.
- Ownership and lifetime: returns newly allocated JSON text that callers must free with the configured JSON free path; internal working buffer is always freed on success or failure.
- Concurrency/global state: allocator/free behavior follows the configured note-c hooks.
- Performance shape: starts with 128 bytes and grows through `_ensure`; final result is tightly sized.
- Portability/platform constraints: omit-empty behavior is entirely controlled by the `omitempty` flag carried in `printbuffer`.

## Direct Call Sites

Exact source search found direct production callers in `JPrint`, `JPrintUnformatted`, and `JPrintUnformattedOmitEmpty`; `NOTE_C_TEST` tests can call `_print` directly.

## Role In Callers

This is the allocating implementation behind public heap-returning JSON print APIs. `JPrintUnformatted` calls it with `format=false` and `omitempty=false`, which is the request serialization mode used by `_noteTransactionShouldLock`.

## Tests Observed

`JPrintUnformatted_test.cpp` and `JSON_number_handling_test.cpp` exercise `_print` through `JPrintUnformatted`. `NoteRequestResponseJSON_test.cpp` uses `_print(json, false, false)` inside a fake for `JPrintUnformatted`.

## Graph Links

- Graph node id: `function:n_cjson.c:983:_print`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused tests were inspected for allocation failure, formatted heap printing, omit-empty heap printing, or direct `_print` failure cleanup.
