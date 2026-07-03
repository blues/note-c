# `JAddBinaryToObject`

## Status

Finalized. Source-inspected Base64 JSON-field writer, ownership paths, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:215`
- Declaration/interface: `note.h:1351`
- Public/exported names: `JAddBinaryToObject`
- Macro aliases/wrappers: none
- Signature/API shape: `bool JAddBinaryToObject(J *json, const char *fieldName, const void *binaryData, uint32_t binaryDataLen)`
- Visibility: public note-c helper API
- Ownership/lifetime role: transfers an allocated Base64 string into a JSON string item on success

## Dependency Status

- Project-local dependencies:
  - `function:n_b64.c:169:JB64EncodeLen` finalized
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_b64.c:174:JB64Encode` finalized
  - `function:n_cjson.c:2374:JCreateStringValue` finalized
  - `macro:n_lib.h:242:_Free` finalized
  - `function:n_cjson.c:1866:JAddItemToObject` finalized
  - `function:n_cjson_helpers.c:19:JIsPresent` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: allocator hooks behind `_Malloc`/`_Free`
- Artifact coverage: source, declaration, caller search, ownership paths, and focused tests inspected

## Behavior

Returns `false` immediately for `json == NULL`. Otherwise computes the encoded length with `JB64EncodeLen`, allocates a string buffer, Base64-encodes `binaryData`, wraps the allocated buffer with `JCreateStringValue`, adds that item to the object with `JAddItemToObject`, and returns `JIsPresent(json, fieldName)`.

## Important Boundaries

- Preconditions and assumptions: `binaryData` must be valid for `binaryDataLen > 0`; `JB64Encode` dereferences it in that case.
- Error/null/empty behavior: allocation failure returns `false`; `JCreateStringValue` failure frees the encoded buffer and returns `false`; null `fieldName` flows to `JAddItemToObject`, which deletes the item, and `JIsPresent` reports false.
- Ownership and lifetime: the encoded buffer is `_Malloc` memory. `JCreateStringValue` adopts the pointer as `valuestring` without copying, and successful object insertion makes the JSON tree responsible for freeing it. If object insertion fails, `JAddItemToObject` deletes the item, which frees non-reference `valuestring`.
- Concurrency/global state: uses process-global allocator hooks through `_Malloc`/`_Free`.
- Portability/platform constraints: pure C helper layered on local Base64 and JSON utilities.
- Performance shape: O(binaryDataLen) encode plus object insertion/lookup cost.
- Security or parsing constraints: no input validation beyond pointer/length assumptions; the function verifies addition by presence of the field, not by comparing encoded contents.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This helper exposes Base64 field creation for public API users.

## Tests Observed

`test/src/JAddBinaryToObject_test.cpp` covers null JSON, allocation failure, `JCreateStringValue` failure, and successful encode/decode round trip.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:215:JAddBinaryToObject`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers null `fieldName`, null `binaryData`, zero-length binary input, or insertion failure after `JCreateStringValue` succeeds.
