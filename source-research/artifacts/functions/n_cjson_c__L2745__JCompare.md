# `JCompare`

## Status

Finalized. Source-inspected public recursive JSON comparison helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2745`
- Declaration/interface: `n_cjson.h:294`
- Public/exported names: `JCompare`
- Macro aliases/wrappers: none found
- Signature/API shape: `Jbool JCompare(const J * const a, const J * const b, const Jbool case_sensitive)`
- Visibility: public cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: read-only recursive comparison; no ownership transfer

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2665:JIsInvalid` finalized
  - `macro:n_cjson.h:320:JArrayForEach` finalized
  - `function:n_cjson.c:1710:_get_object_item` finalized
  - self-recursive calls to `JCompare`
- External/system dependencies: `strcmp` for string/raw comparisons
- Data/type/global dependencies: reads `J` type, number, string, child, next, and object key fields
- Artifact coverage: source-inspected artifact

## Behavior

Returns false when either input is `NULL`, the low-byte base types differ, or `a` is invalid. Rejects unknown base types. Identical pointers compare true after type validity is checked. False, true, and null values compare equal by type alone. Numbers compare with exact `valuenumber ==` equality. Strings and raw values require both `valuestring` pointers to be non-null and `strcmp` equal. Arrays compare child chains positionally and require equal length. Objects compare both directions: every key in `a` must be found in `b` and recursively equal, and every key in `b` must be found in `a` and recursively equal. Object key lookup follows the `case_sensitive` argument through `_get_object_item`.

## Important Boundaries

- Preconditions and assumptions: inputs are expected to be acyclic, well-formed JSON trees with valid string/key pointers where required.
- Error/null/empty behavior: null inputs, invalid left input, mismatched types, unknown types, missing object keys, null string/raw values, unequal numbers, unequal array lengths, or recursive mismatch return false.
- Ownership and lifetime: no allocation, free, or ownership transfer.
- Concurrency/global state: no global state.
- Portability/platform constraints: number equality is exact `JNUMBER` equality.
- Performance shape: recursive tree walk; object comparison is O(n^2) by repeated lookup and performs both subset directions.
- Security or parsing constraints: no depth limit or cycle detection; malformed cycles in child/next graphs can recurse or loop indefinitely.

## Direct Call Sites

Exact source search found the public declaration, the definition, self-recursive array/object comparison calls, and one focused test. No non-self production repository caller was found.

## Role In Callers

No non-self production caller role. Recursive calls compare nested array/object children. Focused test code uses it to assert a transaction response equals a newly-created empty object.

## Tests Observed

Focused evidence in `test/src/NoteTransaction_test.cpp:599` compares a response object with a newly-created empty object using `JCompare(resp, expectedResp, true)`.

## Graph Links

- Graph node id: `function:n_cjson.c:2745:JCompare`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JIsInvalid`, `JArrayForEach`, `_get_object_item`, and self-recursive `JCompare`.

## Evidence Gaps

No unresolved source gaps for this node.
