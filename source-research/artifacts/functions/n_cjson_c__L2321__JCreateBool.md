# `JCreateBool`

## Status

Final. Behavior, declaration, allocation/null behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_cjson.c:2321`
- Declaration/interface: `n_cjson.h:237`
- Public/exported names: `JCreateBool`
- Macro aliases/wrappers: unverified
- Signature/API shape: `J *JCreateBool(Jbool b)`
- Visibility: public cJSON constructor
- Ownership/lifetime role: returns a newly allocated boolean node owned by the caller until attached or deleted

## Dependency Status

- Project-local dependencies:
- `function:n_cjson.c:194:_jNew_Item` (calls, verified; allocates and zeroes a `J` node)
- External/system dependencies: configured cJSON allocator through `_jNew_Item`
- Data/type/global dependencies: none
- Artifact coverage: generated artifact exists

## Behavior

Allocates a new `J` item and sets its type to `JTrue` when `b` is true or `JFalse` when `b` is false. If allocation fails, returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: allocator hooks must be configured for allocation to succeed
- Error/null/empty behavior: allocation failure returns `NULL`
- Ownership and lifetime: caller owns the returned node and must attach it to a parent or delete it
- Concurrency/global state: no global state beyond allocator hooks used by `_jNew_Item`
- Portability/platform constraints: none
- Performance shape: one allocation and one type assignment
- Security or parsing constraints: no parsing or external input handling

## Direct Call Sites

Verified direct production caller: `n_cjson.c:1956` in `JAddBoolToObject`. Focused tests call it in `test/src/JType_test.cpp` and `test/src/JBoolValue_test.cpp`.

## Role In Callers

Boolean-node constructor used directly by tests and by object field creation.

## Tests Observed

Focused evidence:

- `test/src/JType_test.cpp` includes both `JCreateBool(true)` and `JCreateBool(false)` in type-name coverage.
- `test/src/JBoolValue_test.cpp` verifies false and true boolean nodes read back through `JBoolValue`.

## Graph Links

- Graph node id: `function:n_cjson.c:2321:JCreateBool`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test forces allocation failure; null behavior is source-inspected through `_jNew_Item` return handling.
