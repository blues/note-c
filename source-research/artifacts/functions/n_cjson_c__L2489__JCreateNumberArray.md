# `JCreateNumberArray`

## Status

Finalized. Source-inspected public numeric-array constructor.

## Identity

- Kind: function
- Definition: `n_cjson.c:2489`
- Declaration/interface: `n_cjson.h:257`
- Signature/API shape: `J *JCreateNumberArray(const JNUMBER *numbers, int count)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JCreateNumberArray`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2434:JCreateArray` finalized
  - `function:n_cjson.c:2330:JCreateNumber` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson.c:1757:_suffix_object` finalized
- External/system dependencies: none
- Data/type/global dependencies: builds a child list under the returned array
- Artifact coverage: source, header declaration, direct caller search, and focused tests inspected

## Behavior

Returns `NULL` when `count < 0` or `numbers == NULL`. Otherwise creates an empty `JArray`, creates each element with `JCreateNumber(numbers[i])`, links the first child directly and later children with `_suffix_object`, and returns the array. If an element allocation fails, deletes the partially built array and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `numbers` points to at least `count` elements when count is positive.
- Error/null/empty behavior: invalid inputs return `NULL`; `count == 0` returns an empty array when allocation succeeds.
- Ownership and lifetime: caller owns the returned array/tree; partial builds are cleaned with `JDelete` on failure.
- Concurrency/global state: allocation/free follows SDK allocator hook state.
- Portability/platform constraints: numeric representation follows `JNUMBER`.
- Performance shape: O(count).
- Security or parsing constraints: no overflow guard on count-to-size_t cast after the negative check.

## Direct Call Sites

Exact production search found no direct callers outside its header declaration and implementation. It is a public API entry point.

## Role In Callers

No in-repository production caller was found. Public consumers can build an owned JSON array from C numeric input values.

## Tests Observed

No focused test references for this symbol were found under `test/src`.

## Graph Links

- Graph node id: `function:n_cjson.c:2489:JCreateNumberArray`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JCreateArray`, `JCreateNumber`, `JDelete`, and `_suffix_object`

## Evidence Gaps

No unresolved source gaps for this node. Caller status is explicit none-found in repository source.
