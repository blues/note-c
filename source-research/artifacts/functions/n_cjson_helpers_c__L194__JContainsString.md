# `JContainsString`

## Status

Finalized. Source-inspected string-substring field predicate.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:194`
- Declaration/interface: `note.h:1339`
- Signature/API shape: `bool JContainsString(J *json, const char *field, const char *substr)`
- Visibility: public note-c JSON helper
- Public/exported names: `JContainsString`, macro alias `NoteResponseErrorContains`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
  - `function:n_cjson.c:2713:JIsString` finalized
- External/system dependencies: `strlen`, `strstr`
- Data/type/global dependencies: `J.valuestring`

## Behavior

Returns false for null JSON, missing field, non-string field, null `valuestring`, or empty `substr`. Otherwise returns whether `strstr(item->valuestring, substr)` finds a match.

## Important Boundaries

- Preconditions and assumptions: `substr` must be non-null; the function calls `strlen(substr)` without a null guard.
- Error/null/empty behavior: empty substring is rejected rather than treated as a match.
- Ownership and lifetime: no allocation; reads caller-owned JSON tree and substring.
- Concurrency/global state: no global state.
- Performance shape: object lookup plus substring search.
- Portability/platform constraints: bytewise C string search, case-sensitive.

## Direct Call Sites

Exact source search found callers in `_noteTransactionShouldLock` for user-agent gating and response classification, request/response retry tests, and public response-error helper macros.

## Role In Callers

The transaction path uses it to detect API names such as `hub.set`, classify response errors like `{io}`, `{bad-bin}`, `{unsupported}`, and `{heartbeat}`, and preserve retry behavior.

## Tests Observed

`JContainsString_test.cpp` covers null JSON, missing field, wrong type, null value string, empty substring, and successful substring match. Transaction tests exercise error classification indirectly.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:194:JContainsString`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test was inspected for null `field` or null `substr`.
