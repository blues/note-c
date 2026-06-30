# `JB64Decode`

## Status

Finalized. Source-inspected public Base64 decoder.

## Identity

- Kind: function
- Definition: `n_b64.c:121`
- Declaration/interface: `note.h:1516` inside the C-callable `extern "C"` surface beginning at `note.h:107`
- Signature/API shape: `int JB64Decode(char *bufplain, const char *bufcoded)`
- Visibility: public SDK helper declared in `note.h`
- Public/exported names: `JB64Decode`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads static decode table `pr2six[256]`
- Artifact coverage: source, public declaration, direct callers, and focused tests inspected

## Behavior

Scans `bufcoded` until the first non-Base64 byte, computes an expected decoded byte count, then decodes complete 4-byte groups into caller-provided `bufplain`. Handles remaining 2/3/4-byte tails, appends `'\0'`, adjusts the returned decoded length for short tails/padding, and returns the decoded data length excluding the terminator.

The implementation treats a one-byte Base64 remainder as an ignored error case.

## Important Boundaries

- Preconditions and assumptions: `bufplain` and `bufcoded` must be non-`NULL`; `bufcoded` must be null-terminated; `bufplain` must be large enough for decoded bytes plus terminator.
- Error/null/empty behavior: invalid data terminates scanning; there is no explicit error return for invalid input.
- Ownership and lifetime: no allocation; writes into caller-owned output buffer.
- Concurrency/global state: reads immutable static table only.
- Portability/platform constraints: table indexes use `unsigned char` values.
- Performance shape: O(encoded prefix length).
- Security or parsing constraints: no output bounds check; callers must size buffers with `JB64DecodeLen` or equivalent.

## Direct Call Sites

Exact production callers:

- `n_helpers.c:1881` `NotePayloadRetrieveAfterSleep` decodes the restored sleep payload into an allocated buffer and stores it in `NotePayloadDesc`.
- `n_cjson_helpers.c:262` `JGetBinaryFromObject` decodes a JSON string field into caller-returned binary data.

## Role In Callers

Performs the actual decode after callers allocate/own destination storage. Higher-level callers decide whether the resulting buffer is transferred to a descriptor or returned to the API caller.

## Tests Observed

`test/src/JAddBinaryToObject_test.cpp:72` decodes the string created by `JAddBinaryToObject`, checks the decoded length against `sizeof(val)`, and compares decoded bytes to the original string.

## Graph Links

- Graph node id: `function:n_b64.c:121:JB64Decode`
- Structured graph: `inventory/dependency-graph.json`
- Verified callers: `NotePayloadRetrieveAfterSleep`, `JGetBinaryFromObject`

## Evidence Gaps

No unresolved source gaps for this node. Focused tests for invalid Base64 and caller buffer undersizing were not found.
