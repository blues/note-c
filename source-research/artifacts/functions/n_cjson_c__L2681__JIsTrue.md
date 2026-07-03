# `JIsTrue`

## Status

Finalized. Source-inspected JSON true predicate and direct caller.

## Identity

- Kind: function
- Definition: `n_cjson.c:2681`
- Declaration/interface: `n_cjson.h:225`
- Public/exported names: `JIsTrue`
- Macro aliases/wrappers: defined with `N_CJSON_PUBLIC`
- Signature/API shape: `Jbool JIsTrue(const J * const item)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: borrowed read of caller-owned node

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none
- Data/type/global dependencies: reads `J.type`
- Artifact coverage: source, declaration, direct caller, and focused tests inspected

## Behavior

Returns false for null input. Otherwise masks `item->type` with `0xFF` and returns true only when the base type is `JTrue`.

## Important Boundaries

- Preconditions and assumptions: non-null input must point to a valid `J`.
- Error/null/empty behavior: null input returns false.
- Ownership and lifetime: no allocation; reads caller-owned node.
- Concurrency/global state: no global state.
- Portability/platform constraints: none.
- Performance shape: O(1).
- Security or parsing constraints: no parsing.

## Direct Call Sites

Exact production caller: `n_cjson_helpers.c:149` `JGetBool`.

## Role In Callers

`JGetBool` uses this after `JIsBool` accepts the field, so false and missing/non-bool both produce false from the public helper.

## Tests Observed

`test/src/JGetBool_test.cpp` covers true-valued fields through `JGetBool`. No direct predicate-only test was found.

## Graph Links

- Graph node id: `function:n_cjson.c:2681:JIsTrue`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
