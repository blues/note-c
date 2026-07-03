# `JCreateObjectReference`

## Status

Finalized. Source-inspected public object reference wrapper constructor.

## Identity

- Kind: function
- Definition: `n_cjson.c:2394`
- Declaration/interface: `n_cjson.h:252`
- Public/exported names: `JCreateObjectReference`
- Macro aliases/wrappers: none found
- Signature/API shape: `J *JCreateObjectReference(const J *child)`
- Visibility: public cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: allocates a wrapper object node that borrows an existing child chain

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:194:_jNew_Item` finalized
  - `function:n_cjson.c:1826:_cast_away_const` finalized
- External/system dependencies: allocator hook through `_jNew_Item`
- Data/type/global dependencies: writes `item->type` and `item->child`
- Artifact coverage: source-inspected artifact

## Behavior

Returns `NULL` when `child` is `NULL`. Otherwise allocates a zeroed item with `_jNew_Item()`. On allocation success, sets `type = JObject | JIsReference`, stores `child` in `item->child` after `_cast_away_const`, and returns the wrapper. Allocation failure returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `child` must remain valid for as long as the wrapper is used.
- Error/null/empty behavior: null child or allocation failure returns `NULL`; no error state or reason code is set.
- Ownership and lifetime: returned wrapper owns only its allocated `J` node. `JIsReference` prevents `JDelete` from deleting the borrowed child chain.
- Concurrency/global state: no direct global state; allocation depends on the registered allocator hook through `_jNew_Item`.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: O(1) after allocation.
- Security or parsing constraints: removes constness from `child` in the stored pointer and does not validate that the child chain represents an object.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:252` and the definition in `n_cjson.c:2394`. No production repository caller was found.

## Role In Callers

No repository caller role. Public helper for downstream callers that need an object node referencing an existing child chain without transferring child ownership.

## Tests Observed

None. Focused test searches found no direct tests or fakes for `JCreateObjectReference`.

## Graph Links

- Graph node id: `function:n_cjson.c:2394:JCreateObjectReference`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_jNew_Item` and `_cast_away_const`.

## Evidence Gaps

No unresolved source gaps for this node.
