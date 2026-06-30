# `JNumberValue`

## Status

Finalized. Source-inspected JSON item numeric value helper, declaration, direct caller, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:91`
- Declaration/interface: `note.h:1286`
- Public/exported names: `JNumberValue`
- Macro aliases/wrappers: none
- Signature/API shape: `JNUMBER JNumberValue(J *item)`
- Visibility: public note-c JSON helper declared in `note.h`
- Ownership/lifetime role: borrowed read of caller-owned JSON node

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `J.valuenumber`
- Artifact coverage: implementation, public declaration, direct caller, and focused tests inspected

## Behavior

Returns `0.0` for a null item. For any non-null item, returns `item->valuenumber` directly.

## Important Boundaries

- Preconditions and assumptions: non-null `item` must point to a valid `J` node. This helper does not check the node type.
- Error/null/empty behavior: null item returns `0.0`; non-number nodes return whatever `valuenumber` contains, which is normally zero for constructors that do not set a numeric value.
- Ownership and lifetime: no allocation or ownership transfer; reads caller-owned node.
- Concurrency/global state: no global state.
- Portability/platform constraints: numeric precision follows the configured `JNUMBER` type.
- Performance shape: O(1).
- Security or parsing constraints: callers that require type safety must check `JIsNumber` before using this helper.

## Direct Call Sites

Exact source search found direct production caller:

- `n_cjson_helpers.c:111` `JGetNumber` returns the numeric field value after object lookup and type check.

Additional test/HITL sources call this helper directly, but no other production code does.

## Role In Callers

`JGetNumber` uses this as the final value read after it has verified that the requested field exists and is a JSON number.

## Tests Observed

Focused `test/src/JNumberValue_test.cpp` covers null input, a string item returning zero, and a valid number returning its stored `JNUMBER`.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:91:JNumberValue`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming edge from `JGetNumber`.

## Evidence Gaps

No unresolved source gaps for this node.
