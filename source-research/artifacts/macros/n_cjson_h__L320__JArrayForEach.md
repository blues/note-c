# `JArrayForEach`

## Status

Finalized. Source-inspected public iteration macro, structural dependencies, direct production callers, alias, and test evidence search.

## Identity

- Kind: macro
- Definition: `n_cjson.h:320`
- Declaration/interface: public preprocessor macro in `n_cjson.h`
- Public/exported names: `JArrayForEach`
- Macro aliases/wrappers: `JObjectForEach(element, array)` expands to `JArrayForEach(element, array)` at `n_cjson.h:322`
- Signature/API shape: `JArrayForEach(element, array)`
- Visibility: public header macro under the `J_h` include guard
- Ownership/lifetime role: traversal helper only; no allocation, freeing, or ownership transfer

## Dependency Status

- Project-local dependencies:
  - No project-local function callees
- External/system dependencies: `NULL` from the included standard header support
- Data/type/global dependencies: reads the public `J` struct fields `child` and `next`
- Artifact coverage: macro definition, alias, structural fields, direct production callers, and focused test search inspected

## Behavior

Expands to:

```c
for(element = (array != NULL) ? (array)->child : NULL; element != NULL; element = element->next)
```

The initializer sets `element` to `array->child` when `array` is non-null and to `NULL` otherwise. The loop continues while `element` is non-null and advances through the linked sibling chain via `element->next`.

## Important Boundaries

- Preconditions and assumptions: `element` must be a writable lvalue compatible with a `J *`; non-null `array` must point to an object with a valid `child` chain. The `array` expression is evaluated in the initializer and, when non-null, evaluated twice there.
- Error/null/empty behavior: null `array` produces zero iterations. The macro does not validate item type, list integrity, or cycles.
- Ownership and lifetime: no ownership transfer. Loop bodies that free or invalidate `element` before the increment expression runs can make `element->next` unsafe.
- Concurrency/global state: none.
- Portability/platform constraints: preprocessor-only traversal over the public struct layout.
- Performance shape: O(number of siblings traversed).
- Security or parsing constraints: no parsing or bounds checks; depends on valid in-memory JSON list structure.

## Direct Call Sites

Exact production search found:

- `n_cjson.h:320` macro definition.
- `n_cjson.h:322` `JObjectForEach` alias.
- `n_cjson.c:2820` `JCompare` object branch iterates `a` children.
- `n_cjson.c:2834` `JCompare` object branch iterates `b` children to prevent subset-equality.

## Role In Callers

`JCompare` uses this macro only for object comparison. The first pass ensures every field in `a` has a matching equal field in `b`; the second pass ensures `b` has no extra fields absent from `a`.

## Tests Observed

No direct test references to `JArrayForEach` or `JObjectForEach` were found. `test/src/NoteTransaction_test.cpp:599` calls `JCompare` on an expected empty-object response, giving indirect coverage of the object branch's zero-iteration case.

## Test Coverage Notes

Focused tests do not directly cover the macro, non-empty object traversal, alias use through `JObjectForEach`, mutation during iteration, or cyclic/malformed sibling lists.

## Graph Links

- Graph node id: `macro:n_cjson.h:320:JArrayForEach`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
