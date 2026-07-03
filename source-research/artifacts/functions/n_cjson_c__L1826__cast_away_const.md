# `_cast_away_const`

## Status

Finalized. Source-inspected const-cast helper for JSON reference/constant-key paths.

## Identity

- Kind: function
- Definition: `n_cjson.c:1826`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `void *_cast_away_const(const void *string)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Returns the input pointer cast from `const void *` to `void *`.

## Important Boundaries

- Preconditions and assumptions: callers must preserve const-correct ownership semantics separately, usually by setting `JStringIsConst`.
- Error/null/empty behavior: null input returns null after cast.
- Ownership and lifetime: does not allocate, free, or copy.
- Concurrency/global state: no global state.
- Portability/platform constraints: compiler diagnostics around the definition suppress cast-qual warnings for GCC/Clang.
- Performance shape: O(1).

## Direct Call Sites

Exact source search found callers in `n_cjson.c`:

- `n_cjson.c:1845` from `_add_item_to_object` for constant keys.
- `n_cjson.c:2379`, `n_cjson.c:2389`, `n_cjson.c:2402`, and `n_cjson.c:2415` from string/object/array reference constructors.

## Role In Callers

In the active JSON add path, `_add_item_to_object` uses `_cast_away_const` only when `constant_key` is true and then marks the item type with `JStringIsConst`.

## Tests Observed

None. No focused direct test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:1826:_cast_away_const`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
