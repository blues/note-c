# `JAddStringToObject`

## Status

Finalized. Source-inspected JSON string-field add wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:2013`
- Declaration/interface: `n_cjson.h:306`
- Signature/API shape: `J *JAddStringToObject(J * const object, const char * const name, const char * const string)`
- Visibility: cJSON API function implemented in `n_cjson.c`
- Public/exported names: `JAddStringToObject`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2360:JCreateString` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: mutates the target JSON object's child list through `_add_item_to_object`

## Behavior

Returns `NULL` if `object` or `string` is `NULL`. Otherwise creates an owned string node with `JCreateString(string)` and attempts to attach it to `object` under `name` through `_add_item_to_object(object, name, string_item, false)`. On successful attach, returns the new string item. On attach failure, deletes `string_item` and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: `object` must be a valid JSON object-like node; `name` is passed through to `_add_item_to_object`, where null names fail.
- Error/null/empty behavior: null object or string returns `NULL`; allocation/attach failure returns `NULL`.
- Ownership and lifetime: on success, ownership of the new string item transfers to the object; on failure, the temporary item is deleted.
- Concurrency/global state: no global state except allocator/free hooks reached through finalized callees.
- Performance shape: O(length of string + number of existing object children).

## Direct Call Sites

Exact source search found broad production fanout in high-level helpers (`n_helpers.c`), request/error construction (`n_request.c`), user-agent construction (`n_ua.c`), and the active binary transmit path for `"status"`. `NoteNewRequest` uses it to attach the `"req"` field.

## Role In Callers

`NoteNewRequest` depends on `JAddStringToObject` to populate the request object with `c_req`/`"req"` and the caller-provided request name. Higher-level helpers use it for request parameters and response/error fixture construction.

## Tests Observed

Focused tests such as `test/src/JGetString_test.cpp` and many helper tests create object string fields through `JAddStringToObject` and assert downstream retrieval/error behavior. `test/src/NoteAuxSerialFlowControl_test.cpp` includes a fake `NoteNewRequest` implementation that mirrors `JCreateObject` plus `JAddStringToObject`.

## Graph Links

- Graph node id: `function:n_cjson.c:2013:JAddStringToObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JCreateString`, `_add_item_to_object`, and `JDelete`.

## Evidence Gaps

No unresolved source gaps for this node.
