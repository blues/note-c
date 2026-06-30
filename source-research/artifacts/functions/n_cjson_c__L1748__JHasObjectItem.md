# `JHasObjectItem`

## Status

Finalized. Source-inspected public JSON object membership wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1748`
- Declaration/interface: `n_cjson.h:215`
- Public/exported names: `JHasObjectItem`
- Macro aliases/wrappers: none found
- Signature/API shape: `Jbool JHasObjectItem(const J *object, const char *string)`
- Visibility: cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: borrowed read-only object lookup predicate

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: reads object child list through `JGetObjectItem`
- Artifact coverage: source-inspected artifact

## Behavior

Returns `false` when `object` is `NULL`. Otherwise calls `JGetObjectItem(object, string)` and returns `1` when the lookup finds an item, `0` when it returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `string` is passed through to `JGetObjectItem`; a null name is handled by the lower lookup helper as not found.
- Error/null/empty behavior: null object, null name, and missing key all produce false.
- Ownership and lifetime: does not allocate or transfer ownership.
- Concurrency/global state: no global state.
- Portability/platform constraints: no platform-specific behavior.
- Performance shape: O(number of children * key length) via case-insensitive object lookup.
- Security or parsing constraints: no parsing; relies on existing object tree structure.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:215`, the definition in `n_cjson.c:1748`, and one focused test assertion in `test/src/NoteRequestResponseJSON_test.cpp:448`. No production repository caller was found.

## Role In Callers

No production caller role in repository source. The focused test uses the predicate to assert that an error JSON response omits the `id` field.

## Tests Observed

Focused evidence in `test/src/NoteRequestResponseJSON_test.cpp:448` checks `!JHasObjectItem(json, "id")` for a parsed error response.

## Graph Links

- Graph node id: `function:n_cjson.c:1748:JHasObjectItem`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `JGetObjectItem`.

## Evidence Gaps

No unresolved source gaps for this node.
