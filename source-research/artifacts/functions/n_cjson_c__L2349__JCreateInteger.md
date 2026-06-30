# `JCreateInteger`

## Status

Finalized. Source-inspected JSON integer number-node constructor.

## Identity

- Kind: function
- Definition: `n_cjson.c:2349`
- Declaration/interface: not found in `note.h`; public-style `N_CJSON_PUBLIC` definition used by SDK sources
- Signature/API shape: `J *JCreateInteger(JINTEGER integer)`
- Visibility: cJSON API function implemented in `n_cjson.c`
- Public/exported names: `JCreateInteger`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:194:_jNew_Item` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: allocates a `J` node through `_jNew_Item`

## Behavior

Allocates a zeroed `J` item through `_jNew_Item`. If allocation succeeds, sets `type` to `JNumber`, stores the integer converted to `JNUMBER` in `valuenumber`, and stores the original integer in `valueint`. Returns the initialized item or `NULL` on allocation failure.

## Important Boundaries

- Preconditions and assumptions: allocator hook must be installed for allocation to succeed.
- Error/null/empty behavior: allocation failure returns `NULL`.
- Ownership and lifetime: returns caller-owned JSON node; caller must attach it to an owning parent or delete it.
- Concurrency/global state: allocation uses global allocator hook through `_jNew_Item`.
- Portability/platform constraints: numeric value is represented in both `JNUMBER` and `JINTEGER` fields; conversion to `JNUMBER` follows configured numeric type.
- Performance shape: O(1).

## Direct Call Sites

Exact source/graph inspection found the active production caller:

- `n_cjson.c:1995` from `JAddIntToObject`.

## Role In Callers

`JAddIntToObject` uses `JCreateInteger` to allocate the numeric child before attempting object attachment. If attachment fails, `JAddIntToObject` deletes the returned item.

## Tests Observed

None. No focused direct test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:2349:JCreateInteger`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_jNew_Item`.
- Verified incoming edge from `JAddIntToObject`.

## Evidence Gaps

No unresolved source gaps for this node.
