# `JCreateStringValue`

## Status

Finalized. Source-inspected adopted-pointer JSON string constructor and direct caller.

## Identity

- Kind: function
- Definition: `n_cjson.c:2374`
- Declaration/interface: `n_cjson.h:248`
- Public/exported names: `JCreateStringValue`
- Macro aliases/wrappers: defined with `N_CJSON_PUBLIC`
- Signature/API shape: `J *JCreateStringValue(const char *string)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: adopts caller-provided `valuestring` pointer into a JSON node

## Dependency Status

- Project-local dependencies:
- `function:n_cjson.c:194:_jNew_Item` finalized
- `function:n_cjson.c:1826:_cast_away_const` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes `item->type` and `item->valuestring`
- Artifact coverage: source, declaration, direct caller, ownership behavior, and focused tests inspected

## Behavior

Allocates a zeroed `J` with `_jNew_Item`; if allocation succeeds, sets `type = JString` and stores the input pointer in `valuestring` after `_cast_away_const`. It does not duplicate the string and does not set `JIsReference`.

## Important Boundaries

- Preconditions and assumptions: input pointer should be compatible with `JDelete` freeing it through `_Free`, unless ownership is changed before deletion.
- Error/null/empty behavior: allocation failure returns `NULL`; null input can produce a string node with null `valuestring`.
- Ownership and lifetime: returned node owns `valuestring` for deletion because `JIsReference` is not set.
- Concurrency/global state: allocation uses SDK allocator hook.
- Portability/platform constraints: uses `_cast_away_const` to store the pointer in mutable `valuestring`.
- Performance shape: O(1).
- Security or parsing constraints: no string validation or copy.

## Direct Call Sites

Exact production caller: `n_cjson_helpers.c:226` `JAddBinaryToObject`, passing a freshly `_Malloc`-allocated Base64 buffer.

## Role In Callers

Adopts the encoded binary string allocated by `JAddBinaryToObject` so the JSON node owns and frees that buffer.

## Tests Observed

`test/src/JAddBinaryToObject_test.cpp` fakes `JCreateStringValue` for success/failure paths; `test/src/JGetBinaryFromObject_test.cpp` reaches the real binary add/get path. No direct constructor test for null input or deletion ownership was found.

## Graph Links

- Graph node id: `function:n_cjson.c:2374:JCreateStringValue`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node. Note: the nearby header comment groups this with reference strings, but source shows this function is an adopted-pointer owned-value path; `JCreateStringReference` is the one that sets `JIsReference`.
