# `JAllocString`

## Status

Finalized. Source-inspected byte-buffer-to-string allocator, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:342`
- Declaration/interface: `note.h:1390`
- Public/exported names: `JAllocString`
- Macro aliases/wrappers: none
- Signature/API shape: `char *JAllocString(uint8_t *buffer, uint32_t len)`
- Visibility: public note-c helper API
- Ownership/lifetime role: returns caller-owned allocated C string

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:241:_Malloc` finalized
- External/system dependencies: `memcpy`
- Data/type/global dependencies: allocator hooks behind `_Malloc`
- Artifact coverage: source, declaration, caller search, and focused tests inspected

## Behavior

Allocates `len + 1` bytes, returns `NULL` on allocation failure, copies `len` bytes from `buffer` only when `len > 0`, writes a trailing NUL byte at `buf[len]`, and returns the allocated buffer.

## Important Boundaries

- Preconditions and assumptions: `buffer` must be valid when `len > 0`; `buffer == NULL && len > 0` reaches `memcpy`.
- Error/null/empty behavior: allocation failure returns `NULL`; zero length returns an allocated empty string when allocation succeeds.
- Ownership and lifetime: returned buffer is `_Malloc` memory owned by the caller and freed with `JFree`/`NoteFree`.
- Concurrency/global state: uses process-global allocator hooks through `_Malloc`.
- Portability/platform constraints: pure C helper.
- Performance shape: O(len) copy.
- Security or parsing constraints: does not check `len + 1` overflow and does not treat embedded NUL bytes specially.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This helper exposes byte-buffer copying into NUL-terminated storage for public API users.

## Tests Observed

`test/src/JAllocString_test.cpp` covers zero length, allocation failure, and positive-length copy with terminator.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:342:JAllocString`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers null `buffer`, embedded NUL bytes, or `len + 1` overflow.
