# `JGetArraySize`

## Status

Finalized. Source-inspected public array/object child-count helper, macro alias, no-caller search, and focused test search.

## Identity

- Kind: function
- Definition: `n_cjson.c:1660`
- Declaration/interface: `n_cjson.h:208`
- Public/exported names: `JGetArraySize`
- Macro aliases/wrappers: `JGetObjectItems` expands to `JGetArraySize` at `n_cjson.h:209`
- Signature/API shape: `int JGetArraySize(const J *array)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: read-only traversal; no allocation or ownership transfer

## Dependency Status

- Project-local dependencies:
  - No project-local callees
- External/system dependencies: none
- Data/type/global dependencies: reads `J.child` and `J.next`
- Artifact coverage: source, declaration, alias, direct caller search, overflow boundary, and focused test search inspected

## Behavior

Returns 0 for a null `array`. Otherwise starts at `array->child`, walks the sibling chain through `child->next`, increments a `size_t` counter for each child, and returns the count cast to `int`.

## Important Boundaries

- Preconditions and assumptions: non-null input should point to a valid `J` object with an acyclic child chain.
- Error/null/empty behavior: null input and objects/arrays with no child return 0.
- Ownership and lifetime: no allocation, no frees, no ownership transfer.
- Concurrency/global state: none.
- Portability/platform constraints: count is accumulated as `size_t` but returned as `int`; source notes this can overflow without an API break.
- Performance shape: O(number of child nodes).
- Security or parsing constraints: no validation of type, list integrity, or cycles.

## Direct Call Sites

Exact source search found only the public declaration, the `JGetObjectItems` alias, and the implementation. No production callers were found in this repository.

## Role In Callers

No repository production caller currently uses this public count helper. The `JGetObjectItems` alias gives object-count semantics over the same child chain.

## Tests Observed

No direct focused tests for `JGetArraySize` or `JGetObjectItems` were found.

## Test Coverage Notes

Focused tests do not directly cover null input, empty arrays/objects, non-empty traversal, alias use, or return-value overflow.

## Graph Links

- Graph node id: `function:n_cjson.c:1660:JGetArraySize`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
