# `JAddItemToObjectCS`

## Status

Finalized. Source-inspected public constant-key object attach helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1878`
- Declaration/interface: `n_cjson.h:266`
- Public/exported names: `JAddItemToObjectCS`
- Macro aliases/wrappers: none found
- Signature/API shape: `void JAddItemToObjectCS(J *object, const char *string, J *item)`
- Visibility: public cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: transfers `item` into `object` on success while borrowing a constant key string

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
- External/system dependencies: free hook may be reached on cleanup through `JDelete` / `_Free`
- Data/type/global dependencies: marks `item->type` with `JStringIsConst` through `_add_item_to_object`
- Artifact coverage: source-inspected artifact

## Behavior

If `object`, `string`, or `item` is `NULL`, calls `JDelete(item)` and returns. Otherwise calls `_add_item_to_object(object, string, item, true)`. If that helper reports failure, deletes `item`; on success it returns with `item` linked into the object.

## Important Boundaries

- Preconditions and assumptions: caller must supply a key string that outlives the JSON object, matching the header warning for constant-string keys.
- Error/null/empty behavior: invalid inputs consume/delete `item` when non-null; no error status is returned.
- Ownership and lifetime: on success, the object owns `item`; `item->string` borrows `string` and is protected from `JDelete` by `JStringIsConst`.
- Concurrency/global state: no direct global state; cleanup may use the configured free hook through `JDelete`.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: avoids key duplication; append cost is inherited from `_add_item_to_object` / `_add_item_to_array`.
- Security or parsing constraints: no parsing; unsafe if the borrowed key pointer does not remain valid for the JSON object's lifetime.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:266` and the definition in `n_cjson.c:1878`. No production repository caller was found.

## Role In Callers

No repository caller role. Public convenience/API helper for downstream code that wants object insertion without duplicating a constant key string.

## Tests Observed

None. Focused searches found no direct tests for `JAddItemToObjectCS`, `ObjectCS`, or `JStringIsConst`.

## Graph Links

- Graph node id: `function:n_cjson.c:1878:JAddItemToObjectCS`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JDelete` and `_add_item_to_object`.

## Evidence Gaps

No unresolved source gaps for this node.
