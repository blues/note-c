# `_errDoc`

## Status

Finalized. Source-inspected transaction error document builder.

## Identity

- Kind: function
- Definition: `n_request.c:55`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `J *_errDoc(uint32_t id, const char *errmsg)`
- Visibility: internal to `n_request.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2450:JCreateObject` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_cjson.c:1989:JAddIntToObject` finalized
  - `macro:n_lib.h:259:_DebugWithLevel` finalized after debug macro pass
  - `macro:n_lib.h:260:_DebugWithLevelLn` finalized after debug macro pass
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
- External/system dependencies: none
- Data/type/global dependencies: `suppressShowTransactions`, `c_err`

## Behavior

Creates an object, adds `"err": errmsg`, `"src": "note-c"`, and adds `"id": id` only when id is nonzero. When transaction logging is not suppressed, writes a raw error JSON fragment with debug-level functions. If root object allocation fails, logs an allocation error and returns NULL. Add helper failures are not checked.

## Important Boundaries

- Preconditions and assumptions: `errmsg` should be non-null for string-add/debug calls.
- Error/null/empty behavior: root allocation failure returns NULL.
- Ownership and lifetime: caller owns the returned `J` object.
- Concurrency/global state: reads transaction-debug suppression counter and emits through debug hooks.
- Performance shape: bounded object construction.
- Portability/platform constraints: debug macro behavior changes under `NOTE_NODEBUG`.

## Direct Call Sites

Exact source search found callers in `NoteRequestResponseJSON` and `_noteTransactionShouldLock` for transaction-start failure, reset failure, and final transaction errors.

## Role In Callers

Request paths use this helper to return structured error documents with source and optional request ID.

## Tests Observed

`_errDoc_test.cpp` covers allocation failure, suppression, logging under nodebug configuration, source/error fields, and optional ID.

## Graph Links

- Graph node id: `function:n_request.c:55:_errDoc`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
