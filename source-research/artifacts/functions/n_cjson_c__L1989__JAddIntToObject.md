# `JAddIntToObject`

## Status

Finalized. Source-inspected JSON integer field add wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1989`
- Declaration/interface: `n_cjson.h:305`
- Signature/API shape: `J *JAddIntToObject(J * const object, const char * const name, const JINTEGER integer)`
- Visibility: cJSON API function implemented in `n_cjson.c`
- Public/exported names: `JAddIntToObject`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2349:JCreateInteger` finalized
  - `function:n_cjson.c:1835:_add_item_to_object` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: mutates caller-provided JSON object through cJSON child list/key fields

## Behavior

Returns `NULL` immediately if `object` is `NULL`. Otherwise creates a JSON number node with `JCreateInteger(integer)`, attempts to attach it to `object` under `name` using `_add_item_to_object(..., false)`, and returns the new item on success. If attachment fails, it deletes the created item with `JDelete` and returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: caller supplies a mutable JSON object and field name; `name == NULL` causes `_add_item_to_object` failure and cleanup.
- Error/null/empty behavior: null object, allocation failure, null name, or attachment failure returns `NULL`.
- Ownership and lifetime: on success, ownership of the new item transfers to `object`; on failure, this wrapper cleans up the created item.
- Concurrency/global state: allocation/free uses SDK allocator hooks through lower JSON helpers.
- Portability/platform constraints: integer value is stored as both `JINTEGER` and `JNUMBER` by `JCreateInteger`.
- Performance shape: O(strlen(name) + number of existing object children).

## Direct Call Sites

Exact source search found production callers:

- `n_request.c:62` adds an `"id"` field to generated error documents.
- `n_helpers.c:98` and `n_helpers.c:99` add auxiliary serial settings.
- `n_helpers.c:386` and `n_helpers.c:387` add `"offset"` and `"length"` for `NoteBinaryStoreReceive`.
- `n_helpers.c:644` and `n_helpers.c:646` add `"cobs"` and optional `"offset"` for `NoteBinaryStoreTransmit`.

## Role In Callers

In the active receive path, `NoteBinaryStoreReceive` uses `JAddIntToObject` to add request parameters to `card.binary.get` before dispatching `_noteTransactionShouldLock`.

## Tests Observed

Focused numeric evidence:

- `test/src/JSON_number_handling_test.cpp:243` adds `JINTEGER_MAX` and verifies exact JSON printing.
- `test/src/JSON_number_handling_test.cpp:400` adds a Unix timestamp integer and verifies exact JSON printing in a single-precision-sensitive case.

## Graph Links

- Graph node id: `function:n_cjson.c:1989:JAddIntToObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JCreateInteger`, `_add_item_to_object`, and `JDelete`.
- Verified incoming edges from `NoteBinaryStoreReceive`, `NoteBinaryStoreTransmit`, `NoteAuxSerialFlowControl`, and `_errDoc`.

## Evidence Gaps

No unresolved source gaps for this node.
