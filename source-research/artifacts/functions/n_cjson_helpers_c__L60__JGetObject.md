# `JGetObject`

## Status

Final. Behavior, declaration, direct call sites, null/type handling, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:60`
- Declaration/interface: `note.h:1252`
- Public/exported names: `JGetObject`
- Macro aliases/wrappers: `NoteGetBody(a)` expands to `JGetObject(a, "body")` at `note.h:324`
- Signature/API shape: `J *JGetObject(J *json, const char *field)`
- Visibility: public helper API
- Ownership/lifetime role: borrowed child lookup; ownership remains with the parent JSON tree

## Dependency Status

- Project-local dependencies:
- `function:n_cjson.c:1732:JGetObjectItem` (calls, verified)
- `function:n_cjson.c:2729:JIsObject` (calls, verified)
- External/system dependencies: none
- Data/type/global dependencies: none
- Artifact coverage: generated artifact exists

## Behavior

Looks up a named child and returns it only if it exists and is a JSON object. Returns `NULL` when the parent is `NULL`, the field is missing, or the found item is not an object.

## Important Boundaries

- Preconditions and assumptions: `json` should be a valid object tree for successful lookup
- Error/null/empty behavior: `NULL` parent, missing field, and wrong type all return `NULL`; `field` handling is delegated to `JGetObjectItem`
- Ownership and lifetime: returns a borrowed pointer into `json`; caller must not delete it independently unless intentionally detaching elsewhere
- Concurrency/global state: no global state
- Portability/platform constraints: none
- Performance shape: object-member lookup cost plus a type check
- Security or parsing constraints: no parsing; trusts the JSON tree structure supplied by callers

## Direct Call Sites

Verified direct production caller: `n_helpers.c:2366` in `NoteDebugSyncStatus`. Macro alias `NoteGetBody(a)` in `note.h:324` also exposes body lookup. Focused direct test callers are in `test/src/JGetObject_test.cpp`.

## Role In Callers

Typed getter used by higher-level helpers to avoid treating non-object fields as object bodies.

## Tests Observed

`test/src/JGetObject_test.cpp` covers `NULL` parent, missing field, wrong-type field, and valid object field.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:60:JGetObject`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

None for this node.
