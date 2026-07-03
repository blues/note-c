# `_noteTransaction_calculateTimeoutMs`

## Status

Finalized. Source-inspected transaction timeout calculator.

## Identity

- Kind: function
- Definition: `n_request.c:117`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `uint32_t _noteTransaction_calculateTimeoutMs(J *req, bool isReq)`
- Visibility: internal to `n_request.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson_helpers.c:194:JContainsString` finalized
  - `function:n_cjson_helpers.c:19:JIsPresent` finalized
  - `function:n_cjson_helpers.c:122:JGetInt` finalized
  - `macro:note.h:1014:NOTE_C_LOG_DEBUG` finalized after log-macro pass
- External/system dependencies: none
- Data/type/global dependencies: `CARD_INTER_TRANSACTION_TIMEOUT_SEC`

## Behavior

Starts from `(CARD_INTER_TRANSACTION_TIMEOUT_SEC - 1) * 1000`, examines either the `req` or `cmd` field depending on `isReq`, and only honors request-provided timeout fields for APIs containing `note.add` or `web.`. `milliseconds` takes precedence; otherwise `seconds` is multiplied by 1000. The function always adds 1000 ms before returning, making the default effectively `CARD_INTER_TRANSACTION_TIMEOUT_SEC * 1000`.

## Important Boundaries

- Preconditions and assumptions: null or malformed request fields follow the JSON helper defaults.
- Error/null/empty behavior: absent/irrelevant timeout fields return default timeout.
- Ownership and lifetime: no allocation.
- Concurrency/global state: log output depends on configured debug hook/log level.
- Performance shape: object lookups and substring checks only.
- Portability/platform constraints: arithmetic uses `uint32_t`.

## Direct Call Sites

Exact source search found `_noteTransactionShouldLock` as the production caller.

## Role In Callers

The transaction loop uses this timeout for `_Transaction` calls.

## Tests Observed

`_noteTransaction_calculateTimeoutMs_test.cpp` covers `hub.set`, `note.add`, `web.post`, request/command selection, seconds, milliseconds, both fields, and defaults.

## Graph Links

- Graph node id: `function:n_request.c:117:_noteTransaction_calculateTimeoutMs`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
