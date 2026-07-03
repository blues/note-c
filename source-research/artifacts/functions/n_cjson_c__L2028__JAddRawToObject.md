# `JAddRawToObject`

## Status

Finalized. Source-inspected public create-and-add raw-string object helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2028`
- Declaration/interface: `n_cjson.h:307`
- Public/exported names: `JAddRawToObject`
- Macro aliases/wrappers: none found
- Signature/API shape: `J *JAddRawToObject(J * const object, const char * const name, const char * const raw)`
- Visibility: public cJSON helper implemented in `n_cjson.c`
- Ownership/lifetime role: creates a raw JSON item and transfers it into the object on success

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2420:JCreateRaw` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: allocator/free hooks may be reached through callees
- Data/type/global dependencies: allocator/free behavior occurs through callees
- Artifact coverage: source-inspected artifact

## Behavior

Returns `NULL` when `object` or `raw` is `NULL`. Otherwise creates a raw item with `JCreateRaw(raw)`, attempts `_add_item_to_object(object, name, raw_item, false)`, and returns the created item on successful attach. If attach fails, it calls `JDelete(raw_item)` and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `raw` should be a NUL-terminated string; this wrapper does not validate that it contains well-formed JSON.
- Error/null/empty behavior: null object, null raw value, allocation failure, null name, key duplication failure, or attach failure returns `NULL`.
- Ownership and lifetime: on success, the object owns the new raw child, the duplicated key, and the raw node's duplicated `valuestring`; on failure, the temporary child is deleted.
- Concurrency/global state: no direct global state; allocation/free behavior is delegated through callees.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: duplicates `raw`, duplicates `name`, then appends to the object child list.
- Security or parsing constraints: raw string content bypasses JSON validation by design of `JCreateRaw`.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:307`, the definition in `n_cjson.c:2028`, and focused test references. No production repository caller was found.

## Role In Callers

No production repository caller role. Public convenience helper for downstream users that need to attach a raw-valued object field in one call.

## Tests Observed

Focused evidence in `test/src/JGetType_test.cpp` includes `JAddRawToObject` in the string-helper matrix and checks regular, empty, null `valuestring`, numeric-zero, and numeric-nonzero classification behavior through `JGetType`.

## Graph Links

- Graph node id: `function:n_cjson.c:2028:JAddRawToObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JCreateRaw`, `_add_item_to_object`, and `JDelete`.

## Evidence Gaps

No unresolved source gaps for this node.
