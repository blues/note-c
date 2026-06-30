# `JIsExactString`

## Status

Finalized. Source-inspected exact string-field predicate, direct caller, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:173`
- Declaration/interface: `note.h:1329`
- Public/exported names: `JIsExactString`
- Macro aliases/wrappers: none
- Signature/API shape: `bool JIsExactString(J *json, const char *field, const char *teststr)`
- Visibility: public note-c helper API
- Ownership/lifetime role: borrowed read of caller-owned object tree and comparison string

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
  - `function:n_cjson.c:2713:JIsString` finalized
- External/system dependencies: `strlen`, `strcmp`
- Data/type/global dependencies: reads object child `valuestring`
- Artifact coverage: source, declaration, direct caller, and focused tests inspected

## Behavior

Returns `false` when `json` is `NULL`, the field is absent, the item is not a string, the item's `valuestring` is `NULL`, or `teststr` is empty. Otherwise returns whether `strcmp(item->valuestring, teststr) == 0`.

## Important Boundaries

- Preconditions and assumptions: `teststr` must be non-null if execution reaches `strlen(teststr)`.
- Error/null/empty behavior: null object, missing/null field, non-string item, null item string, and empty `teststr` all return `false`.
- Ownership and lifetime: no allocation; reads borrowed object child and borrowed comparison string.
- Concurrency/global state: no global state.
- Portability/platform constraints: uses C library string functions.
- Performance shape: object lookup is O(number of children * key length); comparison is O(length of `teststr` plus compared value).
- Security or parsing constraints: exact byte-wise `strcmp`; no case folding or normalization.

## Direct Call Sites

Production caller found by exact source search:

- `n_request.c:933` `NotePing` validates that an echo response `text` field exactly matches the generated nonce.

## Role In Callers

`NotePing` combines `JIsNullString(rsp, c_err)` with `JIsExactString(rsp, "text", nonce)` so a missing, non-string, null, empty-comparison, or mismatched text field fails the ping validation.

## Tests Observed

`test/src/JIsExactString_test.cpp` covers null JSON, missing field, non-string field, forced null `valuestring`, empty comparison string returning false, and exact match returning true.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:173:JIsExactString`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers `field == NULL`. There is also no guard or test for `teststr == NULL` after a valid string item; that path would reach `strlen(teststr)`.
