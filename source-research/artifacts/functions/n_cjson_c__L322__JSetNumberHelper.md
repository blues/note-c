# `JSetNumberHelper`

## Status

Finalized. Source-inspected numeric value setter helper for the public `JSetNumberValue` macro.

## Identity

- Kind: function
- Definition: `n_cjson.c:322`
- Declaration/interface: `n_cjson.h:316`
- Public/exported names: `JSetNumberHelper`
- Macro aliases/wrappers: `JSetNumberValue(object, number)` in `n_cjson.h:317` delegates to this helper when `object != NULL`
- Signature/API shape: `JNUMBER JSetNumberHelper(J *object, JNUMBER number)`
- Visibility: public helper for cJSON-compatible numeric assignment macro
- Ownership/lifetime role: mutates caller-owned number item fields; no ownership transfer

## Dependency Status

- Project-local dependencies:
- No project-local callees.
- External/system dependencies: none
- Data/type/global dependencies: writes `object->valueint` and `object->valuenumber`
- Artifact coverage: source-inspected artifact; graph repaired with verified macro expansion edge from `JSetNumberValue`

## Behavior

If `object` is `NULL`, returns `number` without writing. Otherwise saturates `object->valueint`: values greater than or equal to `JINTEGER_MAX` store `JINTEGER_MAX`; values less than or equal to `JINTEGER_MIN` store `JINTEGER_MIN`; all other values are cast to `JINTEGER`. It then stores `number` into `object->valuenumber` and returns that value.

## Important Boundaries

- Preconditions and assumptions: caller supplies a mutable `J *` when mutation is desired; helper does not validate that the item is a number.
- Error/null/empty behavior: null object is a no-op that returns the supplied number.
- Ownership and lifetime: no allocation, free, or ownership transfer.
- Concurrency/global state: no global state.
- Portability/platform constraints: saturation limits follow `JINTEGER_MIN` / `JINTEGER_MAX`.
- Performance shape: O(1).
- Security or parsing constraints: no NaN-specific handling; comparisons against integer bounds drive saturation/cast behavior.

## Direct Call Sites

Exact source search found the helper declaration, the implementation, and the public `JSetNumberValue` macro. No direct production repository call or focused test use was found.

## Role In Callers

`JSetNumberValue` uses this helper when `object != NULL`; for null objects, the macro returns `number` without calling the helper.

## Tests Observed

None. Focused test search found no direct `JSetNumberHelper` or `JSetNumberValue` use.

## Graph Links

- Graph node id: `function:n_cjson.c:322:JSetNumberHelper`
- Structured graph: `inventory/dependency-graph.json`
- Verified as a leaf with no project-local callees.
- Verified incoming macro expansion edge from `macro:n_cjson.h:317:JSetNumberValue`.

## Evidence Gaps

No unresolved source gaps for this node.
