# `JCreateRaw`

## Status

Finalized. Source-inspected owned raw-JSON string constructor and direct caller.

## Identity

- Kind: function
- Definition: `n_cjson.c:2420`
- Declaration/interface: `n_cjson.h:242`
- Public/exported names: `JCreateRaw`
- Macro aliases/wrappers: defined with `N_CJSON_PUBLIC`
- Signature/API shape: `J *JCreateRaw(const char *raw)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: returns node owning a duplicated raw JSON string

## Dependency Status

- Project-local dependencies:
- `function:n_cjson.c:194:_jNew_Item` finalized
- `function:n_cjson.c:146:_j_strdup` finalized
- `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: writes `item->type` and `item->valuestring`
- Artifact coverage: source, declaration, direct caller, dependency artifacts, and tests inspected

## Behavior

Allocates a zeroed `J` with `_jNew_Item`; if allocation succeeds, sets `type = JRaw`, duplicates `raw` through `_j_strdup`, and stores the owned copy in `valuestring`. If duplication fails, deletes the partial node with `JDelete` and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: non-null `raw` should be NUL-terminated.
- Error/null/empty behavior: allocation failure, null raw input, or duplicate allocation failure returns `NULL`.
- Ownership and lifetime: returned node owns the duplicated `valuestring`; `JDelete` frees it.
- Concurrency/global state: allocation/free uses SDK allocator hook.
- Portability/platform constraints: allocator routed through SDK hooks.
- Performance shape: O(strlen(raw)).
- Security or parsing constraints: does not validate that `raw` is well-formed JSON.

## Direct Call Sites

Exact production caller: `n_cjson.c:2034` `JAddRawToObject`.

## Role In Callers

Creates the raw child that `JAddRawToObject` attaches to an object or deletes on attach failure.

## Tests Observed

`test/src/JType_test.cpp` constructs a raw node and expects `JType` to report it as `"string"`. No direct allocation-failure or raw-validation test was found.

## Graph Links

- Graph node id: `function:n_cjson.c:2420:JCreateRaw`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
