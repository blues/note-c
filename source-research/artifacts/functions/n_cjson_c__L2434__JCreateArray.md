# `JCreateArray`

## Status

Finalized. Source-inspected public JSON array constructor.

## Identity

- Kind: function
- Definition: `n_cjson.c:2434`
- Declaration/interface: `n_cjson.h:243`
- Signature/API shape: `J *JCreateArray(void)`
- Visibility: public through `N_CJSON_PUBLIC`, whose export/calling-convention shape is configured in `n_cjson.h:134-144`
- Public/exported names: `JCreateArray`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:194:_jNew_Item` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: assigns the `J.type` field to `JArray`; allocation flows through `_jNew_Item` and allocator hook state behind `_Malloc`
- Artifact coverage: source, header declaration, direct callers, and focused tests inspected

## Behavior

Allocates a zeroed `J` node through `_jNew_Item`. If allocation succeeds, sets `item->type = JArray` and returns the new node. If allocation fails, returns `NULL`.

Because `_jNew_Item` zeroes the structure, newly created arrays start with no child elements and no sibling links.

## Important Boundaries

- Preconditions and assumptions: allocator hook must allow `_jNew_Item` to allocate for success.
- Error/null/empty behavior: allocation failure returns `NULL`.
- Ownership and lifetime: returns a caller-owned array node; callers normally release it through `JDelete` or transfer it into another `J` container.
- Concurrency/global state: uses allocator hook state indirectly through `_jNew_Item`.
- Portability/platform constraints: public export macro is platform/config dependent via `N_CJSON_PUBLIC`.
- Performance shape: O(sizeof(J)) allocation and initialization.
- Security or parsing constraints: parsed arrays are created by parser internals, not by this constructor.

## Direct Call Sites

Exact source search found four production callers:

- `n_cjson.c:2080` `JAddArrayToObject` creates an array, attaches it to an object, returns it on success, and deletes it on attach failure.
- `n_cjson.c:2471` `JCreateIntArray` creates an array before appending generated numeric children.
- `n_cjson.c:2500` `JCreateNumberArray` creates an array before appending generated numeric children.
- `n_cjson.c:2530` `JCreateStringArray` creates an array before appending generated string children.

Header/source search also found focused test uses in `test/src/JType_test.cpp:38` and `test/src/JGetArray_test.cpp:28`.

## Role In Callers

This is the public empty-array constructor and the base allocation step for typed array builders and object array-field construction. It establishes the `JArray` type; child population and ownership transfer are handled by callers.

## Tests Observed

- `test/src/JType_test.cpp:30-48` includes `JCreateArray()` in a type-name table and expects `JType` to report `"array"`.
- `test/src/JGetArray_test.cpp:21-45` creates an array, attaches it to an object, and verifies valid retrieval through `JGetArray` along with null, missing, and wrong-type cases.
- `test/src/JGetType_test.cpp:134-137` exercises `JAddArrayToObject`, which depends on `JCreateArray`, and expects `JTYPE_ARRAY`.

## Graph Links

- Graph node id: `function:n_cjson.c:2434:JCreateArray`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge: `_jNew_Item`
- Verified callers: `JAddArrayToObject`, `JCreateIntArray`, `JCreateNumberArray`, `JCreateStringArray`

## Evidence Gaps

No unresolved source gaps for this node. Allocation-failure behavior is source-evident but no focused allocation-failure test was found for this constructor.
