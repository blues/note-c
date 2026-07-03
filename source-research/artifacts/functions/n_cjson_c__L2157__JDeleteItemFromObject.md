# `JDeleteItemFromObject`

## Status

Finalized. Source-inspected public JSON object delete-by-key wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2157`
- Declaration/interface: `n_cjson.h:277`
- Signature/API shape: `void JDeleteItemFromObject(J *object, const char *string)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JDeleteItemFromObject`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2135:JDetachItemFromObject` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: consumes detached `J` ownership through `JDelete`
- Artifact coverage: source, header declaration, direct caller search, and focused tests inspected

## Behavior

Returns immediately when `object` is `NULL`. Otherwise calls `JDelete(JDetachItemFromObject(object, string))`. Null or missing keys flow to `JDelete(NULL)`, which is a no-op.

The API is void and exposes no success/failure status.

## Important Boundaries

- Preconditions and assumptions: does not validate that `object` has type `JObject`.
- Error/null/empty behavior: null object/name or missing key results in no deletion and no status.
- Ownership and lifetime: consumes and frees the detached node/tree when a matching child is found.
- Concurrency/global state: deallocation uses allocator hook state behind `JDelete`/`_Free`.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: lookup cost plus recursive delete cost for the removed subtree.
- Security or parsing constraints: no type validation.

## Direct Call Sites

Exact production search found no direct callers outside its header declaration and implementation. It is a public API entry point.

## Role In Callers

No in-repository production caller was found. Public consumers use it as the consuming/freeing variant of `JDetachItemFromObject`.

## Tests Observed

No focused test references for this symbol were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:2157:JDeleteItemFromObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JDetachItemFromObject` and `JDelete`

## Evidence Gaps

No unresolved source gaps for this node. Caller status is explicit none-found in repository source.
