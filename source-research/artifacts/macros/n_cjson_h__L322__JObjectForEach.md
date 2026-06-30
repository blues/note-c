# `JObjectForEach`

## Status

Finalized. Source-inspected public cJSON object iteration alias macro.

## Identity

- Kind: macro
- Definition: `n_cjson.h:322`
- Declaration/interface: public macro in `n_cjson.h`
- Public/exported names: `JObjectForEach`
- Macro aliases/wrappers: aliases `JArrayForEach(element, array)`
- Signature/API shape: `#define JObjectForEach(element, array) JArrayForEach(element, array)`
- Visibility: public cJSON object-iteration macro
- Ownership/lifetime role: traversal helper only; no allocation, freeing, or ownership transfer

## Dependency Status

- Project-local dependencies:
  - `macro:n_cjson.h:320:JArrayForEach` finalized
- External/system dependencies: none
- Data/type/global dependencies: inherited target macro reads `J.child` and `J.next`
- Artifact coverage: source-inspected artifact with verified macro-alias edge

## Behavior

Expands to `JArrayForEach(element, array)`. The target macro initializes `element` to `array->child` for non-null `array`, initializes it to `NULL` for null `array`, loops while `element` is non-null, and advances through `element->next`.

## Important Boundaries

- Preconditions and assumptions: `element` must be a writable `J *`-compatible lvalue; non-null `array` must point to a valid `J` object/list head.
- Error/null/empty behavior: null `array` produces zero iterations.
- Ownership and lifetime: no ownership transfer. Mutating or freeing the current node inside the loop body can make the target macro's increment expression unsafe.
- Concurrency/global state: none.
- Portability/platform constraints: preprocessor alias over public struct layout traversal.
- Performance shape: O(number of sibling nodes traversed).
- Security or parsing constraints: no validation of object type, child-list integrity, or cycles.

## Direct Call Sites

Exact non-test production search found only the macro definition. No repository production caller of `JObjectForEach` was found.

## Role In Callers

No production repository caller role. Public alias for downstream code that wants object-field iteration naming while reusing `JArrayForEach` traversal mechanics.

## Tests Observed

No exact focused test references to `JObjectForEach` were found. `JArrayForEach` has indirect coverage through `JCompare`, but the object alias itself is not directly exercised.

## Graph Links

- Graph node id: `macro:n_cjson.h:322:JObjectForEach`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing `expands_to` edge to `JArrayForEach`.

## Evidence Gaps

No unresolved source gaps for this node.
