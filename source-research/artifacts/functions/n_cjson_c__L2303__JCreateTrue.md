# `JCreateTrue`

## Status

Finalized. Source-inspected simple JSON true constructor and direct caller.

## Identity

- Kind: function
- Definition: `n_cjson.c:2303`
- Declaration/interface: `n_cjson.h:235`
- Public/exported names: `JCreateTrue`
- Macro aliases/wrappers: defined with `N_CJSON_PUBLIC`
- Signature/API shape: `J *JCreateTrue(void)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: returns newly allocated JSON node owned by caller

## Dependency Status

- Project-local dependencies:
- `function:n_cjson.c:194:_jNew_Item` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes `item->type = JTrue`
- Artifact coverage: source, declaration, direct caller, and focused-test search inspected

## Behavior

Allocates a zeroed `J` with `_jNew_Item`; if allocation succeeds, sets `type = JTrue`; returns the new item or `NULL` on allocation failure.

## Important Boundaries

- Preconditions and assumptions: allocator hook must be configured for allocation to succeed.
- Error/null/empty behavior: allocation failure returns `NULL`.
- Ownership and lifetime: caller owns the returned node and must attach or delete it.
- Concurrency/global state: allocation uses SDK allocator hook through `_jNew_Item`.
- Portability/platform constraints: none beyond allocator hook behavior.
- Performance shape: O(1).
- Security or parsing constraints: no parsing.

## Direct Call Sites

Exact production caller: `n_cjson.c:1917` `JAddTrueToObject`.

## Role In Callers

Creates the child item that `JAddTrueToObject` attaches to an object or deletes on attach failure.

## Tests Observed

No direct focused test for `JCreateTrue` was found.

## Graph Links

- Graph node id: `function:n_cjson.c:2303:JCreateTrue`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
