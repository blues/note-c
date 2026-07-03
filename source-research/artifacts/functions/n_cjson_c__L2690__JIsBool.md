# `JIsBool`

## Status

Finalized. Source-inspected JSON bool predicate and direct caller.

## Identity

- Kind: function
- Definition: `n_cjson.c:2690`
- Declaration/interface: `n_cjson.h:226`
- Public/exported names: `JIsBool`
- Macro aliases/wrappers: defined with `N_CJSON_PUBLIC`
- Signature/API shape: `Jbool JIsBool(const J * const item)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: borrowed read of caller-owned node

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none
- Data/type/global dependencies: reads `J.type`
- Artifact coverage: source, declaration, direct caller, and focused tests inspected

## Behavior

Returns false for null input. Otherwise returns true when any `JTrue` or `JFalse` bit is present in `item->type`, using `(item->type & (JTrue | JFalse)) != 0`.

## Important Boundaries

- Preconditions and assumptions: non-null input must point to a valid `J`.
- Error/null/empty behavior: null input returns false.
- Ownership and lifetime: no allocation; reads caller-owned node.
- Concurrency/global state: no global state.
- Portability/platform constraints: none.
- Performance shape: O(1).
- Security or parsing constraints: unlike most type predicates, this is a bit-presence check rather than exact low-byte equality.

## Direct Call Sites

Exact production caller: `n_cjson_helpers.c:146` `JGetBool`.

## Role In Callers

`JGetBool` uses this to reject missing/non-bool fields before calling `JIsTrue`.

## Tests Observed

`test/src/JGetBool_test.cpp` covers bool lookup behavior through `JGetBool`. No direct predicate-only test for malformed combined bool bits was found.

## Graph Links

- Graph node id: `function:n_cjson.c:2690:JIsBool`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
