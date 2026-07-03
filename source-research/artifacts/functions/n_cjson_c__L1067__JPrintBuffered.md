# `JPrintBuffered`

## Status

Finalized. Source-inspected public buffered JSON print API, finalized allocation/print/free callees, declaration, no-caller search, and focused test search.

## Identity

- Kind: function
- Definition: `n_cjson.c:1067`
- Declaration/interface: `n_cjson.h:199`
- Public/exported names: `JPrintBuffered`
- Macro aliases/wrappers: declared with `N_CJSON_PUBLIC`
- Signature/API shape: `char *JPrintBuffered(const J *item, int prebuffer, Jbool fmt)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: allocates an initial print buffer and returns it to the caller on success

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_cjson.c:1179:_print_value` finalized
  - `macro:n_lib.h:242:_Free` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: `printbuffer` fields; allocator/free hooks through `_Malloc` and `_Free`
- Artifact coverage: source, declaration, finalized callees, no-caller search, ownership behavior, and focused test search inspected

## Behavior

Initializes a zeroed `printbuffer`. Returns `NULL` when `item` is null or `prebuffer` is negative. Allocates `prebuffer` bytes with `_Malloc`; allocation failure returns `NULL`. It then sets the print buffer length, offset, `noalloc=false`, and `format=fmt`, and delegates serialization to `_print_value(item, &p)`. If serialization fails, it frees `p.buffer` and returns `NULL`; otherwise it returns `p.buffer` cast to `char *`.

## Important Boundaries

- Preconditions and assumptions: caller passes either `NULL` or a valid `J` tree; `prebuffer` is a non-negative initial allocation size. A zero `prebuffer` is passed to `_Malloc(0)` and follows the allocator hook's behavior.
- Error/null/empty behavior: null item, negative prebuffer, allocation failure, or `_print_value` failure returns `NULL`; print failure frees the allocated working buffer before returning.
- Ownership and lifetime: successful return transfers ownership of the allocated print buffer to the caller, who must free it with the configured JSON free path such as `JFree`.
- Concurrency/global state: allocation/free behavior follows configured hooks.
- Portability/platform constraints: portable JSON printing; formatting behavior is inherited from `_print_value`.
- Performance shape: starts with caller-specified buffer size and can grow through print internals because `noalloc=false`.
- Security or parsing constraints: does not validate tree integrity before delegating to `_print_value`.

## Direct Call Sites

Exact source search found only the public declaration and source definition. No production callers were found in this repository.

## Role In Callers

No repository production caller currently uses this buffered heap-printing API. It remains public for downstream users that want to supply an initial buffer-size estimate.

## Tests Observed

No direct focused tests for `JPrintBuffered` were found. Its main serializer callee `_print_value` is covered through other print APIs.

## Test Coverage Notes

Focused tests do not directly cover null input, negative or zero prebuffer, allocation failure, print failure cleanup, or formatted/unformatted output through this wrapper.

## Graph Links

- Graph node id: `function:n_cjson.c:1067:JPrintBuffered`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
