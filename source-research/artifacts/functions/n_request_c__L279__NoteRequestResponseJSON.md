# `NoteRequestResponseJSON`

## Status

Finalized. Source-inspected public raw JSON request/response API.

## Identity

- Kind: function
- Definition: `n_request.c:279`
- Declaration/interface: `note.h:407`
- Public/exported names: `NoteRequestResponseJSON`
- Macro aliases/wrappers: none found
- Signature/API shape: `char * NoteRequestResponseJSON(const char *reqJSON)`
- Visibility: public note-c request API implemented in `n_request.c`
- Ownership/lifetime role: sends newline-delimited JSON text and returns a heap-allocated raw JSON response string when one is available

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:228:_TransactionStart` finalized
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
  - `macro:note.h:998:NOTE_C_LOG_WARN` finalized
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_cjson.c:976:JParse` finalized
  - `macro:n_lib.h:242:_Free` finalized
  - `function:n_cjson_helpers.c:19:JIsPresent` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - `macro:n_lib.h:238:_Transaction` finalized
  - `function:n_cjson_helpers.c:122:JGetInt` finalized
  - `function:n_request.c:55:_errDoc` finalized
  - `function:n_cjson.c:1051:JPrintUnformatted` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
  - `macro:n_lib.h:229:_TransactionStop` finalized
- External/system dependencies: `strchr`, `strlen`, `strstr`, `memcpy`
- Data/type/global dependencies: reads `CARD_INTER_TRANSACTION_TIMEOUT_SEC`, `c_bad`, `c_mem`, and JSON field constants; uses transaction, lock, allocation/free, and logging hooks through finalized macros
- Artifact coverage: source-inspected artifact with verified direct call edges

## Behavior

Returns `NULL` immediately for null input or failed transaction start. Otherwise starts a transaction envelope, locks the Notecard, and walks newline-delimited request text manually. If a segment lacks a newline, it allocates a copy with an appended newline for backward compatibility; empty non-newline input logs a bad-request error and exits.

Segments containing the text `"cmd":` are parsed to confirm a top-level command. Commands are sent through `_Transaction` with a null response pointer, then the scanner advances to the next newline-delimited segment. Non-command requests are sent through `_Transaction` with `&rspJSON` and terminate the scan after the response or error handling. If a request transaction returns an error string, the function logs it, attempts to parse the request ID, builds a JSON error document with `_errDoc`, prints it, appends a newline into a newly allocated response buffer, and returns that buffer when allocation succeeds.

The function unlocks the Notecard and stops the transaction envelope before returning. Successful response buffers, including synthesized error JSON buffers, are owned by the caller and must be freed through the configured free hook.

## Important Boundaries

- Preconditions and assumptions: `reqJSON` must point to readable C-string data; command detection begins with a textual `"cmd":` screen, then verifies top-level command presence by parsing.
- Error/null/empty behavior: null input, failed transaction start, empty malformed input, allocation failure, malformed command JSON, and synthesized-error allocation/print failure all return `NULL`.
- Ownership and lifetime: temporary newline-appended input copies are freed before return; returned `rspJSON` is caller-owned; transaction-owned response allocation is delegated to the active transport.
- Concurrency/global state: wraps the full scan in `_LockNote`/`_UnlockNote` after `_TransactionStart`; always calls `_TransactionStop` for paths after successful start.
- Portability/platform constraints: raw transport remains behind `_Transaction`; this API is portable JSON framing and ownership logic.
- Performance shape: scans linearly across command pipelines; malformed non-newline segments allocate one copied buffer; error synthesis parses and prints JSON.
- Security or parsing constraints: command classification avoids parsing unless `"cmd":` appears, so nested `"cmd"` text in non-command requests is parsed and rejected as a top-level command.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:407`, the definition in `n_request.c:279`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public raw-JSON API for downstream code that already has request JSON text rather than `J *` objects.

## Tests Observed

Focused tests in `test/src/NoteRequestResponseJSON_test.cpp` cover null and empty inputs, newline and non-newline commands, command pipelines, missing newline allocation/free behavior, malformed command JSON, requests versus nested command fields, transaction lock/start/stop behavior, transaction-start failure, transaction errors converted to JSON with ID/source, request parse failure during error synthesis, `_errDoc` allocation failure, print failure, and response allocation failure. Gaps: tests do not assert lock/stop counts for every malformed path and use fakes for the transport boundary.

## Graph Links

- Graph node id: `function:n_request.c:279:NoteRequestResponseJSON`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing call edges to transaction envelope macros, Notecard lock macros, transaction dispatch, allocation/free, JSON parse/print helpers, error document creation, and logging helpers.

## Evidence Gaps

No unresolved source gaps for this node.
