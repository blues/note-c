# `JDuplicate`

## Status

Finalized. Source-inspected public shallow/deep JSON item duplication helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2550`
- Declaration/interface: `n_cjson.h:288`
- Public/exported names: `JDuplicate`
- Macro aliases/wrappers: none found
- Signature/API shape: `J *JDuplicate(const J *item, Jbool recurse)`
- Visibility: public cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: allocates a new JSON item/tree that the caller owns and must release

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:194:_jNew_Item` finalized
  - `function:n_cjson.c:146:_j_strdup` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - self-recursive call to `JDuplicate(child, true)`
- External/system dependencies: allocator/free hooks through callees
- Data/type/global dependencies: reads source item fields and reconstructs child `next`/`prev` links
- Artifact coverage: source-inspected artifact

## Behavior

Returns `NULL` for `item == NULL` or any allocation/copy failure. Otherwise allocates a new zeroed item, copies `type` with `JIsReference` cleared, copies numeric fields, duplicates `valuestring` when present, and duplicates `string` unless the source item has `JStringIsConst`, in which case it reuses the original key pointer. When `recurse` is false, returns this shallow duplicate without children. When `recurse` is true, recursively duplicates each child in the source `child`/`next` chain and rebuilds the duplicate child's `next` and `prev` links.

## Important Boundaries

- Preconditions and assumptions: source tree/list is expected to be acyclic and well-formed.
- Error/null/empty behavior: null input, allocation failure, string duplication failure, or recursive child duplication failure returns `NULL`.
- Ownership and lifetime: returned duplicate owns copied nodes and copied strings except const key strings reused from source when `JStringIsConst` is set.
- Concurrency/global state: no direct global state; allocation/free behavior is delegated through callees.
- Portability/platform constraints: no platform-specific behavior in this wrapper.
- Performance shape: shallow mode is O(size of current strings); recursive mode is O(number of reachable child/sibling nodes plus copied string lengths).
- Security or parsing constraints: no depth limit, no cycle detection, and no use of `N_CJSON_NESTING_LIMIT`; malformed or cyclic child/next graphs are not guarded.

## Direct Call Sites

Exact production source search found only the public declaration in `n_cjson.h:288`, the definition in `n_cjson.c:2550`, and the self-recursive call in `n_cjson.c:2589`. No non-self production repository caller was found.

## Role In Callers

No non-self repository caller role. The only in-repo call site is recursive duplication of child nodes when `recurse` is true.

## Tests Observed

None. Focused test search found no direct `JDuplicate` tests or fakes.

## Graph Links

- Graph node id: `function:n_cjson.c:2550:JDuplicate`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_jNew_Item`, `_j_strdup`, `JDelete`, and self-recursive `JDuplicate`.

## Evidence Gaps

No unresolved source gaps for this node.
