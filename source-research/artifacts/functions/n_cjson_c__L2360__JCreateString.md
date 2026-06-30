# `JCreateString`

## Status

Finalized. Source-inspected owned JSON string constructor.

## Identity

- Kind: function
- Definition: `n_cjson.c:2360`
- Declaration/interface: `n_cjson.h:240`
- Signature/API shape: `J *JCreateString(const char *string)`
- Visibility: cJSON API function implemented in `n_cjson.c`
- Public/exported names: `JCreateString`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:194:_jNew_Item` finalized
  - `function:n_cjson.c:146:_j_strdup` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: writes `item->type` and `item->valuestring`

## Behavior

Allocates a new JSON node through `_jNew_Item`. If allocation succeeds, sets `type = JString`, duplicates the input string through `_j_strdup`, and stores the owned copy in `valuestring`. If string duplication fails, deletes the partially constructed node with `JDelete` and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `string` is expected to be a valid NUL-terminated string; `_j_strdup` returns `NULL` for null input or allocation failure.
- Error/null/empty behavior: allocation failure or duplication failure returns `NULL`.
- Ownership and lifetime: returned node owns `valuestring`; `JDelete` frees it unless reference flags are later set externally.
- Concurrency/global state: allocation/free paths depend on finalized hook allocator behavior.
- Performance shape: O(length of string).

## Direct Call Sites

Exact source search found production callers:

- `JAddStringToObject` at `n_cjson.c:2019`.
- `JCreateStringArray` at `n_cjson.c:2533`.

Focused tests also construct string nodes for JSON value/type helpers.

## Role In Callers

`JAddStringToObject` uses `JCreateString` to create an owned string child before attaching it to an object. `JCreateStringArray` uses it for array elements.

## Tests Observed

Focused tests in `test/src/JStringValue_test.cpp`, `test/src/JType_test.cpp`, and related JSON helper tests exercise created string nodes and verify string/type behavior through public helper APIs.

## Graph Links

- Graph node id: `function:n_cjson.c:2360:JCreateString`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_jNew_Item`, `_j_strdup`, and `JDelete`.

## Evidence Gaps

No unresolved source gaps for this node.
