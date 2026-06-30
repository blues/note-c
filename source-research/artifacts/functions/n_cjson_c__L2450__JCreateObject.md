# `JCreateObject`

## Status

Finalized. Source-inspected JSON object constructor.

## Identity

- Kind: function
- Definition: `n_cjson.c:2450`
- Declaration/interface: `n_cjson.h:244`
- Signature/API shape: `J *JCreateObject(void)`
- Visibility: cJSON API function implemented in `n_cjson.c`
- Public/exported names: `JCreateObject`
- Macro aliases/wrappers: `NoteNewBody` expands to `JCreateObject` at `note.h:322`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:194:_jNew_Item` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes `item->type`

## Behavior

Allocates a zeroed JSON node through `_jNew_Item`. If allocation succeeds, sets `item->type = JObject` and returns the new node. If allocation fails, returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: allocator hooks must be configured for `_jNew_Item` to allocate.
- Error/null/empty behavior: allocation failure returns `NULL`.
- Ownership and lifetime: caller owns the returned `J` and releases it with `JDelete`.
- Concurrency/global state: allocation path depends on finalized hook allocator behavior.
- Performance shape: O(1).

## Direct Call Sites

Exact source search found production callers in request/error construction (`n_request.c`), JSON object add wrappers (`n_cjson.c`), user-agent construction (`n_ua.c`), and the public `NoteNewBody` macro alias in `note.h`.

## Role In Callers

`NoteNewRequest` uses `JCreateObject` to allocate the request document before attaching the `"req"` field. Other request and helper paths use it to construct response, command, user-agent, body, and nested object nodes.

## Tests Observed

Focused tests use `JCreateObject` in many JSON/helper setup paths. `test/src/JType_test.cpp` verifies that an object created by `JCreateObject` reports the object type, and `test/src/NoteNewRequest_test.cpp` verifies caller behavior when `JCreateObject` succeeds or fails.

## Graph Links

- Graph node id: `function:n_cjson.c:2450:JCreateObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_jNew_Item`.

## Evidence Gaps

No unresolved source gaps for this node.
