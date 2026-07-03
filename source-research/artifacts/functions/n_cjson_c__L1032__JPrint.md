# `JPrint`

## Status

Finalized. Source-inspected public formatted JSON print wrapper, finalized `_print` callee, declaration, no-caller search, and focused test search.

## Identity

- Kind: function
- Definition: `n_cjson.c:1032`
- Declaration/interface: `n_cjson.h:193`
- Public/exported names: `JPrint`
- Macro aliases/wrappers: declared with `N_CJSON_PUBLIC`
- Signature/API shape: `char *JPrint(const J *item)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: returns caller-owned allocated JSON text when successful

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:983:_print` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: allocation and free behavior are inherited from `_print` through configured JSON hooks
- Artifact coverage: source, declaration, finalized callee, no-caller search, ownership behavior, and focused test search inspected

## Behavior

Returns `NULL` when `item` is `NULL`. Otherwise calls `_print(item, true, false)` and casts the returned `unsigned char *` to `char *`. The `format=true` argument requests formatted JSON output; `omitempty=false` means false, zero, and empty-string fields are not omitted by this wrapper.

## Important Boundaries

- Preconditions and assumptions: caller passes either `NULL` or a valid `J` tree.
- Error/null/empty behavior: null input returns `NULL`; allocation or serialization failure returns `NULL` through `_print`.
- Ownership and lifetime: successful return is newly allocated JSON text; callers free it with the configured JSON free path such as `JFree`.
- Concurrency/global state: allocator/free behavior follows configured hooks inherited from `_print`.
- Portability/platform constraints: portable JSON printing; formatting details are inherited from cJSON print internals.
- Performance shape: uses `_print`'s allocating buffer-growth strategy and final tight copy.
- Security or parsing constraints: does not validate tree integrity before delegating to `_print`.

## Direct Call Sites

Exact source search found only the public declaration, source definition, and the memory-management note in `n_cjson.h`. No production callers were found in this repository.

## Role In Callers

No repository production caller currently uses this formatted heap-printing API. It remains public for downstream users of the JSON API.

## Tests Observed

No direct focused tests for `JPrint` were found. Printing behavior is exercised elsewhere through `JPrintUnformatted` and `_print`, but not through this formatted wrapper.

## Test Coverage Notes

Focused tests do not directly cover formatted printing through `JPrint`, null input, or allocation-failure propagation through this wrapper.

## Graph Links

- Graph node id: `function:n_cjson.c:1032:JPrint`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
