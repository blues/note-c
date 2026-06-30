# `JB64EncodeLen`

## Status

Finalized. Source-inspected public Base64 encoded-length helper.

## Identity

- Kind: function
- Definition: `n_b64.c:169`
- Declaration/interface: `note.h:1489` inside the C-callable `extern "C"` surface beginning at `note.h:107`
- Signature/API shape: `int JB64EncodeLen(int len)`
- Visibility: public SDK helper declared in `note.h`
- Public/exported names: `JB64EncodeLen`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: none
- Artifact coverage: source, public declaration, direct callers, and focused tests inspected

## Behavior

Returns `((len + 2) / 3 * 4) + 1`, which is the encoded character count rounded to a four-byte Base64 block plus a null terminator.

## Important Boundaries

- Preconditions and assumptions: caller supplies a non-negative input length in the representable `int` range.
- Error/null/empty behavior: `len == 0` returns `1`.
- Ownership and lifetime: no allocation; caller owns any buffer sized from the result.
- Concurrency/global state: none.
- Portability/platform constraints: integer arithmetic uses `int`; no overflow guard.
- Performance shape: O(1).
- Security or parsing constraints: negative or very large lengths are not guarded.

## Direct Call Sites

Exact production callers:

- `n_helpers.c:1716` `NotePayloadSaveAndSleep` sizes a temporary Base64 payload buffer.
- `n_cjson_helpers.c:220` `JAddBinaryToObject` sizes a JSON string backing buffer.

## Role In Callers

Provides allocation size for higher-level payload and JSON-binary encode paths. The callers allocate and free/transfer the buffer.

## Tests Observed

No direct focused tests for `JB64EncodeLen` were found. `test/src/JAddBinaryToObject_test.cpp:72` verifies the encode/decode round trip produced by the `JAddBinaryToObject` path.

## Graph Links

- Graph node id: `function:n_b64.c:169:JB64EncodeLen`
- Structured graph: `inventory/dependency-graph.json`
- Verified callers: `NotePayloadSaveAndSleep`, `JAddBinaryToObject`

## Evidence Gaps

No unresolved source gaps for this node. Focused tests for negative/overflow lengths were not found.
