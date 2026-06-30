# `JIsFalse`

## Status

Finalized. Source-inspected public false-type predicate.

## Identity

- Kind: function
- Definition: `n_cjson.c:2673`
- Declaration/interface: `n_cjson.h:224`
- Public/exported names: `JIsFalse`
- Macro aliases/wrappers: none found
- Signature/API shape: `Jbool JIsFalse(const J * const item)`
- Visibility: public cJSON predicate implemented in `n_cjson.c`
- Ownership/lifetime role: read-only type predicate; no ownership transfer

## Dependency Status

- Project-local dependencies:
- No project-local callees.
- External/system dependencies: none
- Data/type/global dependencies: reads `item->type`
- Artifact coverage: source-inspected artifact

## Behavior

Returns `false` when `item` is `NULL`. Otherwise masks `item->type` with `0xFF` and returns true when the base type is `JFalse`.

## Important Boundaries

- Preconditions and assumptions: caller may pass any `J *`; only the low type byte is inspected.
- Error/null/empty behavior: null input returns false.
- Ownership and lifetime: no allocation, free, or ownership transfer.
- Concurrency/global state: no global state.
- Portability/platform constraints: no platform-specific behavior.
- Performance shape: O(1).
- Security or parsing constraints: ignores flag bits such as `JIsReference` and `JStringIsConst` by masking with `0xFF`.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:224` and the definition in `n_cjson.c:2673`. No production repository caller was found.

## Role In Callers

No repository caller role. Public cJSON-compatible predicate for downstream callers.

## Tests Observed

None. Focused test search found no direct `JIsFalse` use.

## Graph Links

- Graph node id: `function:n_cjson.c:2673:JIsFalse`
- Structured graph: `inventory/dependency-graph.json`
- Verified as a leaf with no project-local callees.

## Evidence Gaps

No unresolved source gaps for this node.
