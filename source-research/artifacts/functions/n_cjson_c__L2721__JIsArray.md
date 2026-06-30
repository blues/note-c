# `JIsArray`

## Status

Finalized. Source-inspected JSON array predicate and direct caller.

## Identity

- Kind: function
- Definition: `n_cjson.c:2721`
- Declaration/interface: `n_cjson.h:230`
- Public/exported names: `JIsArray`
- Macro aliases/wrappers: defined with `N_CJSON_PUBLIC`
- Signature/API shape: `Jbool JIsArray(const J * const item)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: borrowed read of caller-owned node

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none
- Data/type/global dependencies: reads `J.type`
- Artifact coverage: source, declaration, direct caller, and focused tests inspected

## Behavior

Returns false for null input. Otherwise masks `item->type` with `0xFF` and returns true only when the base type is `JArray`.

## Important Boundaries

- Preconditions and assumptions: non-null input must point to a valid `J`.
- Error/null/empty behavior: null input returns false.
- Ownership and lifetime: no allocation; reads caller-owned node.
- Concurrency/global state: no global state.
- Portability/platform constraints: none.
- Performance shape: O(1).
- Security or parsing constraints: exact base-type check after masking flags, so `JArray | JIsReference` still passes.

## Direct Call Sites

Exact production caller: `n_cjson_helpers.c:54` `JGetArray`.

## Role In Callers

`JGetArray` uses this to reject missing/non-array fields before returning a borrowed child pointer.

## Tests Observed

`test/src/JGetArray_test.cpp` covers null, missing, wrong-type, and valid-array lookup through `JGetArray`. No direct predicate-only test was found.

## Graph Links

- Graph node id: `function:n_cjson.c:2721:JIsArray`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
