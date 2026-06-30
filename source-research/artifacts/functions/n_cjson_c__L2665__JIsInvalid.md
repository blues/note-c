# `JIsInvalid`

## Status

Finalized. Source-inspected JSON invalid-type predicate and direct caller.

## Identity

- Kind: function
- Definition: `n_cjson.c:2665`
- Declaration/interface: `n_cjson.h:223`
- Public/exported names: `JIsInvalid`
- Macro aliases/wrappers: defined with `N_CJSON_PUBLIC`
- Signature/API shape: `Jbool JIsInvalid(const J * const item)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: borrowed read of caller-owned node

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none
- Data/type/global dependencies: reads `J.type`
- Artifact coverage: source, declaration, direct caller, and focused-test search inspected

## Behavior

Returns false for null input. Otherwise masks `item->type` with `0xFF` and returns true only when the base type is `JInvalid`.

## Important Boundaries

- Preconditions and assumptions: non-null input must point to a valid `J`.
- Error/null/empty behavior: null input returns false.
- Ownership and lifetime: no allocation; reads caller-owned node.
- Concurrency/global state: no global state.
- Portability/platform constraints: none.
- Performance shape: O(1).
- Security or parsing constraints: no parsing.

## Direct Call Sites

Exact production caller: `n_cjson.c:2747` `JCompare`.

## Role In Callers

`JCompare` rejects null, type-mismatched, and invalid left-hand JSON nodes before value comparison.

## Tests Observed

No direct focused test for `JIsInvalid` was found.

## Graph Links

- Graph node id: `function:n_cjson.c:2665:JIsInvalid`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
