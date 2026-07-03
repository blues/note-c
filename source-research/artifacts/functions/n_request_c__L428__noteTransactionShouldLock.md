# `_noteTransactionShouldLock`

## Status

Finalized. Source-inspected Notecard JSON transaction orchestrator.

## Identity

- Kind: function
- Definition: `n_request.c:428`
- Declaration/interface: `n_lib.h:123`
- Signature/API shape: `J *_noteTransactionShouldLock(J *req, bool lockNotecard)`
- Visibility: internal request helper shared with binary helpers and tests
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
  - `function:n_cjson.c:1051:JPrintUnformatted` finalized
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `macro:n_lib.h:242:_Free` finalized
  - `function:n_cjson_helpers.c:122:JGetInt` finalized
  - `macro:n_lib.h:228:_TransactionStart` finalized
  - `function:n_request.c:55:_errDoc` finalized
  - `function:n_cjson_helpers.c:19:JIsPresent` finalized
  - `function:n_cjson_helpers.c:194:JContainsString` finalized
  - `function:n_ua.c:95:NoteUserAgent` finalized
  - `function:n_cjson.c:1866:JAddItemToObject` finalized
  - `macro:note.h:76:NOTE_C_LOG_DEBUG` finalized
  - `macro:note.h:1014:NOTE_C_LOG_DEBUG` finalized
  - `function:n_request.c:117:_noteTransaction_calculateTimeoutMs` finalized
  - `function:n_request.c:1047:_crcAdd` finalized
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:237:_Reset` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
  - `macro:n_lib.h:229:_TransactionStop` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - `macro:note.h:1006:NOTE_C_LOG_INFO` finalized
  - `macro:n_lib.h:238:_Transaction` finalized
  - `function:n_request.c:940:NoteErrorContains` finalized
  - `macro:note.h:998:NOTE_C_LOG_WARN` finalized
  - `macro:n_lib.h:244:_DelayMs` finalized
  - `function:n_request.c:1113:_crcError` finalized
  - `function:n_cjson.c:976:JParse` finalized
  - `macro:n_lib.h:259:_DebugWithLevel` finalized
  - `function:n_hooks.c:643:_noteHeartbeat` finalized
  - `function:n_request.c:756:NoteResetRequired` finalized
  - `function:n_cjson.c:2450:JCreateObject` finalized
- External/system dependencies: `strlen`
- Data/type/global dependencies: `resetRequired`, `seqNo`, `suppressShowTransactions`, retry constants, error string constants

## Behavior

Rejects null requests, serializes the input `J` with `JPrintUnformatted`, and requires exactly one of `req` or `cmd`. It captures the request ID, starts the transaction, optionally adds a user-agent body for `hub.set` with product when enabled, computes the transaction timeout, optionally locks the Notecard, captures the current CRC sequence number as `transactionSeqNo`, optionally adds request CRC in non-low-memory request paths, and performs any queued interface reset before I/O.

The retry loop logs outgoing JSON when transaction display is not suppressed, temporarily replaces the serialized buffer terminator with `\n`, calls `_Transaction` with no response for commands or a response pointer for requests, restores the terminator, and classifies outcomes. Retryable cases include I/O transaction errors, null responses, CRC errors, invalid JSON responses, and response `{io}` errors excluding `{not-supported}`. `{bad-bin}` errors are not retried. Heartbeat responses free the raw response, do not count against retry count, and may invoke `_noteHeartbeat` when the callback feature is enabled.

After the loop it frees the serialized request and increments global `seqNo` when CRC support is compiled. The response CRC check uses the captured `transactionSeqNo`, so retry validation stays tied to the request CRC even though `seqNo` advances before the Notecard lock is released. The function then returns an empty object for commands, returns `_errDoc` on request error after marking reset required, or logs/frees raw response JSON and returns the parsed response object on success.

## Important Boundaries

- Preconditions and assumptions: caller owns `req`; function does not delete it.
- Error/null/empty behavior: command errors generally return NULL before command completion or an empty object after command send; request errors return structured error objects when possible.
- Ownership and lifetime: owns and frees serialized request JSON; owns and frees raw response JSON on success/error after parsing; transfers returned `J *` to caller.
- Concurrency/global state: optionally takes Notecard lock before CRC insertion; mutates `resetRequired`, advances `seqNo` before unlock in locked transactions, and reads transaction logging suppression state.
- Performance shape: bounded retry loop over `CARD_REQUEST_RETRIES_ALLOWED`, with JSON serialization, transport I/O, optional CRC, and parse per attempt.
- Portability/platform constraints: CRC branches are excluded under `NOTE_C_LOW_MEM`; heartbeat callback branch requires `NOTE_C_HEARTBEAT_CALLBACK`; transport/reset/locking are behind hooks/adapters.
- Source caveat: user-agent body insertion happens after `json` is serialized, and this source does not reserialize before `_Transaction`, so that mutation is not reflected in the transmitted buffer.

## Direct Call Sites

Exact source search found direct production callers in `NoteTransaction`, `NoteBinaryStoreReceive`, and `NoteBinaryStoreTransmit`. Tests also call or fake this helper in `NoteTransaction_test.cpp`, `NoteBinaryStoreReceive_test.cpp`, and `NoteBinaryStoreTransmit_test.cpp`.

## Role In Callers

`NoteTransaction` passes `lockNotecard=true` for normal public JSON request flow. Binary store receive/transmit pass `lockNotecard=false` because they already hold the Notecard lock around binary transfer setup.

## Tests Observed

`NoteTransaction_test.cpp` covers null/malformed requests, start failure, lock true/false paths, sequence advancement before Notecard unlock, reset failure, I/O retries, bad-bin no-retry, invalid JSON retry, CRC paths, user-agent guards, command behavior, and heartbeat behavior. `_noteTransaction_calculateTimeoutMs_test.cpp`, `_crcAdd_test.cpp`, `_crcError_test.cpp`, `_errDoc_test.cpp`, hook tests, and binary store tests cover important callee branches.

## Graph Links

- Graph node id: `function:n_request.c:428:_noteTransactionShouldLock`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
