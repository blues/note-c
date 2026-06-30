# `JB64Encode`

## Status

Finalized. Source-inspected public Base64 encoder.

## Identity

- Kind: function
- Definition: `n_b64.c:174`
- Declaration/interface: `note.h:1499` inside the C-callable `extern "C"` surface beginning at `note.h:107`
- Signature/API shape: `int JB64Encode(char *encoded, const char *string, int len)`
- Visibility: public SDK helper declared in `note.h`
- Public/exported names: `JB64Encode`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads static alphabet `basis_64`
- Artifact coverage: source, public declaration, direct callers, and focused tests inspected

## Behavior

Writes Base64 output for complete three-byte groups using `basis_64`, emits `=` padding for one- or two-byte tails, appends `'\0'`, and returns `p - encoded`. The returned count includes the null terminator.

## Important Boundaries

- Preconditions and assumptions: `encoded` and `string` must be non-`NULL`; `encoded` must be large enough for `JB64EncodeLen(len)` bytes; `len` is expected non-negative.
- Error/null/empty behavior: `len == 0` writes only `'\0'` and returns `1`.
- Ownership and lifetime: no allocation; writes into caller-owned output buffer.
- Concurrency/global state: reads immutable static alphabet only.
- Portability/platform constraints: input is `const char *`; bit operations on signed `char` values are partly masked but signed-char behavior remains worth noting for binary data portability.
- Performance shape: O(len).
- Security or parsing constraints: no output bounds check or invalid argument guard.

## Direct Call Sites

Exact production callers:

- `n_helpers.c:1725` `NotePayloadSaveAndSleep` encodes a sleep-state payload before calling `NoteSleep`.
- `n_cjson_helpers.c:225` `JAddBinaryToObject` encodes binary data into a JSON string backing buffer.

## Role In Callers

Performs encode after callers allocate destination storage. Higher-level callers either free the temporary encoded buffer or transfer it into a JSON string value.

## Tests Observed

`test/src/JAddBinaryToObject_test.cpp:72` decodes the string produced by `JAddBinaryToObject`, checks decoded length against `sizeof(val)`, and compares decoded bytes to the original string.

## Graph Links

- Graph node id: `function:n_b64.c:174:JB64Encode`
- Structured graph: `inventory/dependency-graph.json`
- Verified callers: `NotePayloadSaveAndSleep`, `JAddBinaryToObject`

## Evidence Gaps

No unresolved source gaps for this node. Focused tests for zero-length, non-text binary bytes, and signed-char edge cases were not found.
