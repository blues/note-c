# `JGetObjectItems`

## Status

Finalized. Source-inspected public cJSON object-size alias macro.

## Identity

- Kind: macro
- Definition: `n_cjson.h:209`
- Declaration/interface: public macro in `n_cjson.h`
- Public/exported names: `JGetObjectItems`
- Macro aliases/wrappers: aliases `JGetArraySize`
- Signature/API shape: `#define JGetObjectItems JGetArraySize`
- Visibility: public cJSON compatibility/helper macro
- Ownership/lifetime role: no ownership; forwards object-child counting to the array-size helper

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1660:JGetArraySize` finalized
- External/system dependencies: none
- Data/type/global dependencies: inherits `JGetArraySize` traversal of `J.child` and `J.next`
- Artifact coverage: source-inspected artifact with verified macro-alias edge

## Behavior

Expands `JGetObjectItems(...)` to `JGetArraySize(...)`. The target function returns `0` for a null argument, counts linked children through `child`/`next`, and casts the final `size_t` count to `int`.

## Important Boundaries

- Preconditions and assumptions: argument is evaluated according to the target function call; non-null input should point to a valid `J` list head.
- Error/null/empty behavior: null input returns `0` through `JGetArraySize`.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: preprocessor alias; target API return type is `int`.
- Performance shape: O(number of child nodes).
- Security or parsing constraints: target function notes the count cast can overflow for extremely large child lists.

## Direct Call Sites

Exact non-test production search found only the macro definition. No repository production caller of `JGetObjectItems` was found.

## Role In Callers

No production repository caller role. Public alias for downstream code that wants object item count terminology while reusing the array-size implementation.

## Tests Observed

No exact focused test references to `JGetObjectItems` were found. Tests for `JGetArraySize` behavior provide only indirect target-function coverage.

## Graph Links

- Graph node id: `macro:n_cjson.h:209:JGetObjectItems`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing `expands_to` edge to `JGetArraySize`.

## Evidence Gaps

No unresolved source gaps for this node.
