# `JCreateFalse`

## Status

Finalized. Source-inspected simple JSON false constructor and direct caller.

## Identity

- Kind: function
- Definition: `n_cjson.c:2312`
- Declaration/interface: `n_cjson.h:236`
- Public/exported names: `JCreateFalse`
- Macro aliases/wrappers: defined with `N_CJSON_PUBLIC`
- Signature/API shape: `J *JCreateFalse(void)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: returns newly allocated JSON node owned by caller

## Dependency Status

- Project-local dependencies:
- `function:n_cjson.c:194:_jNew_Item` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes `item->type = JFalse`
- Artifact coverage: source, declaration, direct caller, and focused-test search inspected

## Behavior

Allocates a zeroed `J` with `_jNew_Item`; if allocation succeeds, sets `type = JFalse`; returns the new item or `NULL` on allocation failure.

## Important Boundaries

- Preconditions and assumptions: allocator hook must be configured for allocation to succeed.
- Error/null/empty behavior: allocation failure returns `NULL`.
- Ownership and lifetime: caller owns the returned node and must attach or delete it.
- Concurrency/global state: allocation uses SDK allocator hook through `_jNew_Item`.
- Portability/platform constraints: none beyond allocator hook behavior.
- Performance shape: O(1).
- Security or parsing constraints: no parsing.

## Direct Call Sites

Exact production caller: `n_cjson.c:1932` `JAddFalseToObject`.

## Role In Callers

Creates the child item that `JAddFalseToObject` attaches to an object or deletes on attach failure.

## Tests Observed

No direct focused test for `JCreateFalse` was found.

## Graph Links

- Graph node id: `function:n_cjson.c:2312:JCreateFalse`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
