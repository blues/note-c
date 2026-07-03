# `JB64DecodeLen`

## Status

Finalized. Source-inspected public Base64 decoded-length helper.

## Identity

- Kind: function
- Definition: `n_b64.c:106`
- Declaration/interface: `note.h:1507` inside the C-callable `extern "C"` surface beginning at `note.h:107`
- Signature/API shape: `int JB64DecodeLen(const char *bufcoded)`
- Visibility: public SDK helper declared in `note.h`
- Public/exported names: `JB64DecodeLen`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads static decode table `pr2six[256]`
- Artifact coverage: source, public declaration, direct callers, and focused tests inspected

## Behavior

Treats `bufcoded` as a null-terminated Base64 string and scans bytes through `pr2six` until the first non-Base64 byte. Computes the decoded capacity as `((nprbytes + 3) / 4) * 3` and returns that value plus one byte for a null terminator.

## Important Boundaries

- Preconditions and assumptions: `bufcoded` must be non-`NULL`, null-terminated, and readable; no guard exists.
- Error/null/empty behavior: empty or immediately invalid input returns `1`; invalid data after a prefix is treated as the end of encoded input.
- Ownership and lifetime: no allocation; caller owns all buffers.
- Concurrency/global state: reads immutable static table only.
- Portability/platform constraints: table indexes use `unsigned char` values.
- Performance shape: O(encoded prefix length).
- Security or parsing constraints: no validity/error reporting, output bounds check, or overflow guard for very large inputs.

## Direct Call Sites

Exact production callers:

- `n_helpers.c:1875` `NotePayloadRetrieveAfterSleep` sizes the allocation used for a restored sleep payload.
- `n_cjson_helpers.c:258` `JGetBinaryFromObject` sizes the allocation for decoded binary data plus its own convenience terminator.

## Role In Callers

Provides decode buffer capacity for higher-level payload and JSON-binary helpers. The callers allocate memory and transfer ownership independently.

## Tests Observed

No direct focused tests for `JB64DecodeLen` were found. `test/src/JAddBinaryToObject_test.cpp:72` uses `JB64Decode` to verify an encode/decode round trip through `JAddBinaryToObject`.

## Graph Links

- Graph node id: `function:n_b64.c:106:JB64DecodeLen`
- Structured graph: `inventory/dependency-graph.json`
- Verified callers: `NotePayloadRetrieveAfterSleep`, `JGetBinaryFromObject`

## Evidence Gaps

No unresolved source gaps for this node. Focused tests for invalid input and overflow behavior were not found.
