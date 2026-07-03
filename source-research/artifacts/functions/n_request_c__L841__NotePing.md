# `NotePing`

## Status

Finalized. Source-inspected public fast connectivity ping API.

## Identity

- Kind: function
- Definition: `n_request.c:841`
- Declaration/interface: `note.h:314`
- Public/exported names: `NotePing`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NotePing(void)`
- Visibility: public note-c request API implemented in `n_request.c`
- Ownership/lifetime role: constructs a one-shot echo request, owns all temporary JSON buffers, and returns only success/failure

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:243:_GetMs` finalized
  - `function:n_cjson.c:2450:JCreateObject` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_cjson.c:1051:JPrintUnformatted` finalized
  - `function:n_cjson.c:209:JDelete` finalized
  - `function:n_request.c:88:_noteSuspendTransactionDebug` finalized
  - `macro:n_lib.h:228:_TransactionStart` finalized
  - `macro:n_lib.h:242:_Free` finalized
  - `function:n_request.c:80:_noteResumeTransactionDebug` finalized
  - `macro:n_lib.h:226:_LockNote` finalized
  - `function:n_request.c:786:_notePingDrainInput` finalized
  - `macro:n_lib.h:238:_Transaction` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
  - `macro:n_lib.h:229:_TransactionStop` finalized
  - `function:n_cjson.c:976:JParse` finalized
  - `function:n_cjson_helpers.c:152:JIsNullString` finalized
  - `function:n_cjson_helpers.c:173:JIsExactString` finalized
- External/system dependencies: `strlen`
- Data/type/global dependencies: uses `c_req`, `c_err`, transaction/debug suppression state through helper functions, and active transport state through `_notePingDrainInput` and `_Transaction`
- Artifact coverage: source-inspected artifact with verified direct call edges

## Behavior

Generates a 16-character uppercase nonce with xorshift32 seeded from the current millisecond clock. Builds an `{"req":"echo","text":nonce}` JSON object, prints it, deletes the object, overwrites the printed string's terminating NUL with a newline, and sends it with explicit `jsonLen + 1`.

Before sending, suppresses normal transaction debug output. It starts a short 500 ms transaction envelope, locks the Notecard, drains residual bytes from the active transport, and performs exactly one `_Transaction` call. It deliberately does not honor `resetRequired`, does not call `_Reset`, and does not add a CRC. It unlocks, stops the transaction, resumes debug output, frees the request JSON, and then parses the response. The function returns true only when the response parses as JSON, has no effective `"err"` string, and contains a `"text"` field exactly matching the nonce.

## Important Boundaries

- Preconditions and assumptions: active transport hooks must already be configured for `_Transaction`; allocation hooks must support the JSON object/print path.
- Error/null/empty behavior: object allocation, print failure, transaction-start failure, transaction error, no response, invalid JSON, response error, or nonce mismatch all return false.
- Ownership and lifetime: request JSON, response JSON, and parsed response objects are freed on handled paths; no response object is returned to the caller.
- Concurrency/global state: locks the Notecard around drain and transaction; temporarily increments/decrements transaction-debug suppression through helper functions.
- Portability/platform constraints: transport-specific draining stays in `_notePingDrainInput`; the public API remains transport-independent.
- Performance shape: one short fixed-timeout transaction, no retry, no reset; serial drain has its own bounded quiet window and I2C drain consumes queued data.
- Security or parsing constraints: nonce is generated for echo matching, not cryptographic security; response validation ignores unrelated fields.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:314`, the definition in `n_request.c:840`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public fast connectivity check intended for downstream probing flows such as serial autobaud detection.

## Tests Observed

Focused tests in `test/src/NotePing_test.cpp` cover matching echo success, wrong nonce, response error, invalid JSON, transaction error, no response, no reset when `resetRequired` is set, transaction-start failure without lock/transaction, serial input drain, and I2C drain under I2C lock. Gaps: allocation failures in the request construction/print path are not directly exercised.

## Graph Links

- Graph node id: `function:n_request.c:841:NotePing`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing call edges to time, JSON construction/print/parse/delete helpers, debug suppression helpers, transaction envelope macros, lock macros, drain helper, transaction dispatch, free macro, and response validators.

## Evidence Gaps

No unresolved source gaps for this node.
