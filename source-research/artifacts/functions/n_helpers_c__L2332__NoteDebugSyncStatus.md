# `NoteDebugSyncStatus`

## Status

Final. Behavior, public declaration, direct call sites, state/timing behavior, request/response ownership, debug suppression, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_helpers.c:2332`
- Declaration/interface: `note.h:423`
- Public/exported names: `NoteDebugSyncStatus`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteDebugSyncStatus(int pollFrequencyMs, int maxLevel)`
- Visibility: public SDK helper
- Ownership/lifetime role: creates a request consumed by `NoteRequestResponse`; owns and deletes any returned response before returning

## Dependency Status

- Project-local dependencies:
- `macro:n_lib.h:243:_GetMs` (calls, verified)
- `function:n_request.c:173:NoteNewRequest` (calls, verified)
- `function:n_cjson.c:2013:JAddStringToObject` (calls, verified)
- `function:n_cjson.c:1950:JAddBoolToObject` (calls, verified)
- `function:n_request.c:88:_noteSuspendTransactionDebug` (calls, verified)
- `function:n_request.c:220:NoteRequestResponse` (calls, verified; consumes request)
- `function:n_request.c:80:_noteResumeTransactionDebug` (calls, verified)
- `macro:note.h:479:NoteResponseError` (calls, verified)
- `macro:note.h:486:NoteDeleteResponse` (calls, verified)
- `function:n_cjson_helpers.c:60:JGetObject` (calls, verified)
- `function:n_cjson_helpers.c:122:JGetInt` (calls, verified)
- `macro:n_lib.h:256:_Debug` (calls, verified)
- `function:n_cjson_helpers.c:27:JGetString` (calls, verified)
- `macro:n_lib.h:257:_Debugln` (calls, verified)
- External/system dependencies: Notecard `note.get` request for `_synclog.qi`
- Data/type/global dependencies: static local `lastCommStatusPollMs`
- Artifact coverage: generated artifact exists

## Behavior

Polls `_synclog.qi` for one queued sync-status note and optionally emits it to the debug stream. If a previous no-status/error poll recorded `lastCommStatusPollMs` and the unsigned elapsed time is less than `(uint32_t)pollFrequencyMs`, it returns `false` without making a request.

When polling, it creates a `note.get` request, sets `"file":"_synclog.qi"` and `"delete":true`, suppresses transaction debug output around `NoteRequestResponse(req)`, and resumes debug output immediately after the request returns. A `NULL` request or `NULL` response returns `false`.

If the response has an error, it records `lastCommStatusPollMs = _GetMs()`, deletes the response, and returns `false`. Otherwise it looks for object field `"body"`. If a body exists and `maxLevel < 0` or `JGetInt(body, "level") <= maxLevel`, it writes `[SYNC] `, the body `subsystem`, a space, and the body `text` with a trailing newline. It deletes the response and returns `true` even when no body is present or the level is above `maxLevel`.

## Important Boundaries

- Preconditions and assumptions: intended to be called periodically by applications that want sync-status debug output
- Error/null/empty behavior: no request allocation or no response returns `false`; response error returns `false` and starts the suppression interval; no body returns `true`; missing body fields flow through `JGetString`/debug helpers
- Ownership and lifetime: `NoteRequestResponse` consumes the request; this function deletes any non-`NULL` response exactly once
- Concurrency/global state: uses static `lastCommStatusPollMs` without locking; transaction debug suppression counter is incremented/decremented around the request
- Portability/platform constraints: debug output can be compiled away by `NOTE_NODEBUG` through `_Debug`/`_Debugln`
- Performance shape: normally one Notecard request per unsuppressed call; quick no-request return while suppressed
- Security or parsing constraints: no untrusted parsing beyond JSON helpers; debug output may include Notecard-provided subsystem/text fields

## Direct Call Sites

No production library callers found by `rg -n "\bNoteDebugSyncStatus\b" --glob '!source-research/**'`. References are the public declaration in `note.h:423`, definition in `n_helpers.c:2332`, and focused unit tests.

## Role In Callers

Application-facing polling helper for surfacing Notecard sync-log entries as debug output.

## Tests Observed

`test/src/NoteDebugSyncStatus_test.cpp` covers:

- `NoteNewRequest` failure, `NoteRequestResponse` failure, and response-error failure
- suppression of repeated polls after an error and unsigned millisecond rollover behavior
- success with no response body
- valid body with level above, at, below, and unrestricted by negative `maxLevel`
- debug output expectations under normal and `NOTE_NODEBUG` builds

## Graph Links

- Graph node id: `function:n_helpers.c:2332:NoteDebugSyncStatus`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused test covers negative `pollFrequencyMs`; source casts it to `uint32_t` in the suppression comparison.
