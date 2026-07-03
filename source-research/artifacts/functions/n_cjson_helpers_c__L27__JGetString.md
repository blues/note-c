# `JGetString`

## Status

Finalized. Source-inspected JSON object string-field accessor.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:27`
- Declaration/interface: `note.h:1210`
- Signature/API shape: `char *JGetString(J *json, const char *field)`
- Visibility: public helper declaration in `note.h`, implemented in `n_cjson_helpers.c`
- Public/exported names: `JGetString`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
  - `function:n_cjson.c:2713:JIsString` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: reads the target JSON object's child list and string value pointer

## Behavior

Returns `c_nullstring` when `json` is `NULL`, the field is absent, the field is not a string, or the field's `valuestring` is `NULL`. Otherwise returns the field's `valuestring` pointer.

## Important Boundaries

- Preconditions and assumptions: returned non-empty pointer is borrowed from the JSON object; caller must not use it after the object is deleted.
- Error/null/empty behavior: missing/non-string/null-value cases all collapse to the shared empty string.
- Ownership and lifetime: returns borrowed memory or shared constant empty string; does not allocate or copy.
- Concurrency/global state: reads `c_nullstring` constant and JSON object state.
- Performance shape: O(number of object children * key length).

## Direct Call Sites

Exact source search found broad production fanout:

- Request routing/error handling in `n_request.c`.
- Binary store helpers, time/location/env/status/service/contact/payload/debug helpers in `n_helpers.c`.
- Binary/base64 helper in `n_cjson_helpers.c`.

## Role In Callers

In the active receive path, `NoteBinaryStoreReceive` calls `JGetString(rsp, "err")` for transaction error logging and `JGetString(rsp, "status")` to copy the expected MD5 string before receiving binary data.

## Tests Observed

None. No focused direct `JGetString` test was inspected in this slice.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:27:JGetString`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JGetObjectItem` and `JIsString`.

## Evidence Gaps

No unresolved source gaps for this node.
