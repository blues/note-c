# `_create_reference`

## Status

Finalized. Source-inspected JSON shallow reference wrapper constructor.

## Identity

- Kind: function
- Definition: `n_cjson.c:1764`
- Declaration/interface: file-local `NOTE_C_STATIC`; no public header declaration
- Signature/API shape: `J *_create_reference(const J *item)`
- Visibility: internal to `n_cjson.c` unless statics are exposed for tests
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:194:_jNew_Item` finalized
- External/system dependencies: `memcpy`
- Data/type/global dependencies: copies `J` node fields and sets `JIsReference`
- Artifact coverage: source, direct callers, ownership behavior, and focused test search inspected

## Behavior

Returns `NULL` when `item` is `NULL`. Otherwise allocates a new zeroed `J` node with `_jNew_Item`; allocation failure returns `NULL`. On success, shallow-copies the source `J` into the new node, clears `reference->string`, sets `JIsReference` in the type flags, clears `next` and `prev`, and returns the wrapper.

The referenced child/value storage is not duplicated.

## Important Boundaries

- Preconditions and assumptions: non-null `item` points to a valid `J`.
- Error/null/empty behavior: null input or allocation failure returns `NULL`.
- Ownership and lifetime: caller owns the wrapper node, but the wrapper does not own referenced child/value storage because `JIsReference` prevents recursive child/value freeing in `JDelete`.
- Concurrency/global state: allocation follows SDK allocator hook state through `_jNew_Item`.
- Portability/platform constraints: shallow copy assumes stable `J` layout.
- Performance shape: O(sizeof(J)).
- Security or parsing constraints: no deep-copy validation; lifetime of referenced data must outlive the reference wrapper.

## Direct Call Sites

Exact production callers:

- `n_cjson.c:1894` `JAddItemReferenceToArray`.
- `n_cjson.c:1905` `JAddItemReferenceToObject`.

## Role In Callers

Creates the non-owning wrapper that lets a JSON array/object refer to an existing `J` item without corrupting or transferring ownership of the original item.

## Tests Observed

No focused test references for this helper or public reference-add wrappers were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:1764:_create_reference`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_jNew_Item`

## Evidence Gaps

No unresolved source gaps for this node. Focused tests for reference wrapper deletion semantics were not found.
