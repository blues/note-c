# `JIsObject`

## Status

Final. Behavior, declaration, direct call sites, null/type behavior, and indirect focused coverage have been inspected.

## Identity

- Kind: function
- Definition: `n_cjson.c:2729`
- Declaration/interface: `n_cjson.h:231`
- Public/exported names: `JIsObject`
- Macro aliases/wrappers: unverified
- Signature/API shape: `Jbool JIsObject(const J * const item)`
- Visibility: public cJSON predicate
- Ownership/lifetime role: borrowed inspection only; no ownership

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none
- Data/type/global dependencies: reads `item->type`
- Artifact coverage: generated artifact exists

## Behavior

Returns `false` for `NULL`. Otherwise masks the low byte of `item->type` and returns whether it equals `JObject`.

## Important Boundaries

- Preconditions and assumptions: non-`NULL` input should point to a valid `J`
- Error/null/empty behavior: `NULL` returns `false`
- Ownership and lifetime: does not allocate, retain, or mutate
- Concurrency/global state: no global state
- Portability/platform constraints: none
- Performance shape: constant-time type check
- Security or parsing constraints: no parsing; trusts the `J` pointer when non-`NULL`

## Direct Call Sites

Verified direct production caller: `n_cjson_helpers.c:68` in `JGetObject`.

## Role In Callers

Low-level type predicate used by typed object getter helpers.

## Tests Observed

No focused test calls `JIsObject` by name. `test/src/JGetObject_test.cpp` indirectly covers object, non-object, and null/missing behavior through `JGetObject`.

## Graph Links

- Graph node id: `function:n_cjson.c:2729:JIsObject`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

None for this node.
