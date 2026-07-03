# `JGetBinaryFromObject`

## Status

Finalized. Source-inspected Base64 JSON-field reader, direct payload mode, ownership paths, caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:235`
- Declaration/interface: `note.h:1371`
- Public/exported names: `JGetBinaryFromObject`
- Macro aliases/wrappers: none
- Signature/API shape: `bool JGetBinaryFromObject(J *json, const char *fieldName, uint8_t **retBinaryData, uint32_t *retBinaryDataLen)`
- Visibility: public note-c helper API
- Ownership/lifetime role: allocates caller-owned decoded buffer on success

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_b64.c:106:JB64DecodeLen` finalized
  - `function:n_b64.c:121:JB64Decode` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: allocator hooks behind `_Malloc`
- Artifact coverage: source, declaration, caller search, ownership paths, and focused tests inspected

## Behavior

Initializes `*retBinaryData` to `NULL` and `*retBinaryDataLen` to zero before checking `json`. If `json` is `NULL`, returns `false`. If `fieldName` is `NULL`, treats `json` itself as a Base64 payload string pointer. Otherwise retrieves the named string field with `JGetString`. Empty payloads return `false`. Non-empty payloads allocate `JB64DecodeLen(payload) + 1`, decode with `JB64Decode`, append a convenience NUL byte after the decoded data, store the output pointer and decoded length, and return `true`.

## Important Boundaries

- Preconditions and assumptions: `retBinaryData` and `retBinaryDataLen` must be valid output pointers because the function writes through them before validation. In direct payload mode, `json` must actually be a valid C string pointer cast to `J *`.
- Error/null/empty behavior: null `json`, missing/non-string/null string field through `JGetString`, empty payload, and allocation failure return `false` with outputs left null/zero.
- Ownership and lifetime: successful output buffer is `_Malloc` memory owned by the caller and freed with `JFree`/`NoteFree`.
- Concurrency/global state: uses process-global allocator hooks through `_Malloc`.
- Portability/platform constraints: pure C helper layered on local Base64 and JSON utilities.
- Performance shape: O(encoded payload length) decode plus optional object lookup.
- Security or parsing constraints: invalid Base64 is not explicitly rejected; the local decoder stops at the first non-Base64 character and returns the decoded prefix length.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This helper exposes Base64 field decoding and direct payload-string decoding for public API users.

## Tests Observed

`test/src/JGetBinaryFromObject_test.cpp` covers null JSON, empty string field, allocation failure, successful named-field decode, and successful direct payload-string mode.

## Test Coverage Notes

Focused tests do not cover null output pointers, absent field, non-string field, invalid Base64, or direct payload mode with an invalid/non-string pointer.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:235:JGetBinaryFromObject`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
