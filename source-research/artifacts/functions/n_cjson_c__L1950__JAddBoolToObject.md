# `JAddBoolToObject`

## Status

Final. Behavior, declaration, direct call sites, ownership, error handling, and focused test usage have been inspected.

## Identity

- Kind: function
- Definition: `n_cjson.c:1950`
- Declaration/interface: `n_cjson.h:303`
- Public/exported names: `JAddBoolToObject`
- Macro aliases/wrappers: unverified
- Signature/API shape: `J *JAddBoolToObject(J * const object, const char * const name, const Jbool boolean)`
- Visibility: public cJSON helper
- Ownership/lifetime role: creates a boolean `J` child and transfers ownership to `object` on successful attachment; deletes the created child on attach failure

## Dependency Status

- Project-local dependencies:
- `function:n_cjson.c:2321:JCreateBool` (calls, verified; creates `JTrue`/`JFalse`)
- `function:n_cjson.c:1835:_add_item_to_object` (calls, verified; attaches child under `name`)
- `function:n_cjson.c:209:JDelete` (calls on attach failure, verified)
- External/system dependencies: allocator through `JCreateBool`
- Data/type/global dependencies: none
- Artifact coverage: generated artifact exists

## Behavior

Adds a boolean field to an object. If `object == NULL`, returns `NULL` without allocating. Otherwise creates a boolean item with `JCreateBool(boolean)`, attempts to attach it with `_add_item_to_object(object, name, bool_item, false)`, returns the new item on success, and deletes the new item plus returns `NULL` on failure.

## Important Boundaries

- Preconditions and assumptions: `object` should be a valid object node for successful attachment; the helper itself only checks for `NULL`
- Error/null/empty behavior: `NULL` object returns `NULL`; allocation failure or attach failure returns `NULL`; `name` validation is delegated to `_add_item_to_object`
- Ownership and lifetime: caller owns the parent; on success the parent owns the new child, on failure this function frees the new child
- Concurrency/global state: no global state
- Portability/platform constraints: none beyond configured cJSON allocator hooks
- Performance shape: one allocation plus object-child insertion cost
- Security or parsing constraints: no parsing; object/key validity is delegated to lower-level object insertion

## Direct Call Sites

Verified direct production callers in `n_helpers.c`: request builders for delete/reset/start/align/sync flags at `496`, `562`, `1487`, `1843`, `1909`, `2003`, `2038`, `2045`, `2100`, and sync-status polling at `2347`. Test callers use it to construct boolean fields in `JGetBool`, `JGetType`, `NoteGetStatus`, and `NoteIsConnected` tests.

## Role In Callers

Convenience constructor/attacher for boolean request/response fields. Callers generally ignore its return value after constructing best-effort request JSON.

## Tests Observed

Focused evidence:

- `test/src/JGetBool_test.cpp` verifies boolean fields added with this helper are read as booleans.
- `test/src/JGetType_test.cpp` verifies true and false fields are typed as `JTYPE_BOOL_TRUE` and `JTYPE_BOOL_FALSE`.

## Graph Links

- Graph node id: `function:n_cjson.c:1950:JAddBoolToObject`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test directly forces `JCreateBool` or `_add_item_to_object` failure through this helper; failure behavior is source-inspected.
