# `NoteSleep`

## Status

Finalized. Source-inspected public sleep helper, dependency-ready callees, direct production caller, declaration, and focused tests.

## Identity

- Kind: function
- Definition: `n_helpers.c:1754`
- Declaration/interface: `note.h:1977`
- Public/exported names: `NoteSleep`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteSleep(char *stateb64, uint32_t seconds, const char *modes)`
- Visibility: public SDK helper declared in `note.h`
- Ownership/lifetime role: consumes a caller-owned command object through `NoteRequest`; may attach a borrowed payload string reference before request deletion

## Dependency Status

- Project-local dependencies:
  - `macro:note.h:1006:NOTE_C_LOG_INFO` finalized
  - `function:n_request.c:182:NoteNewCommand` finalized
  - `function:n_cjson.c:2384:JCreateStringReference` finalized
  - `function:n_cjson.c:1866:JAddItemToObject` finalized
  - `function:n_str.c:31:strlcpy` finalized
  - `function:n_str.c:69:strlcat` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_cjson.c:1974:JAddNumberToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
- External/system dependencies: none
- Data/type/global dependencies: logging macros depend on configured SDK log hooks; JSON allocation and request behavior follow previously finalized allocator/transport layers
- Artifact coverage: source, declaration, direct production caller, finalized callees, ownership boundaries, and focused tests inspected

## Behavior

Initializes `success` to false, logs that sleep is about to begin, and creates a `card.attn` command via `NoteNewCommand`. If command allocation fails, it skips request construction and returns false. On a non-null command, it optionally creates a `JCreateStringReference` for non-null `stateb64` and adds it as `"payload"` when the reference node allocation succeeds. It then builds a fixed 64-byte mode string beginning with `"sleep"`, appends `","` and `modes` when `modes` is non-null using bounded `strlcat`, adds `"mode"` and `"seconds"` fields, and sends the command with `NoteRequest`. The returned value is exactly the bool result from `NoteRequest` on the command path. The final `NOTE_C_LOG_ERROR("DIDN'T SLEEP\n")` call is unconditional.

## Important Boundaries

- Preconditions and assumptions: `stateb64` and `modes` are optional pointers; `seconds` is copied as a JSON number. Long `modes` strings are truncated by the 64-byte local buffer and `strlcat` result is ignored.
- Error/null/empty behavior: null command object returns false and does not call `NoteRequest`; null payload omits `"payload"`; failure to allocate the payload reference silently omits `"payload"`; field-add failures for `"mode"` or `"seconds"` are not checked before calling `NoteRequest`.
- Ownership and lifetime: `JCreateStringReference` stores a borrowed `stateb64` pointer; after adding it to the command, `NoteRequest` consumes/deletes the command before `NoteSleep` returns, so the borrowed pointer only needs to remain valid through the call.
- Concurrency/global state: no local static state; log and request behavior use global SDK hook layers.
- Portability/platform constraints: portable C helper; platform-specific behavior stays behind log, allocator, JSON, and request/transport hooks.
- Performance shape: O(length of optional mode string plus JSON field creation); avoids copying large payload state by using a string reference.
- Security or parsing constraints: no validation of mode tokens; long mode input is bounded/truncated by `strlcat`.

## Direct Call Sites

Exact source search found one production caller and public/test references:

- `n_helpers.c:1730` `NotePayloadSaveAndSleep` base64-encodes an optional binary payload, calls `NoteSleep(stateB64, seconds, modes)`, frees the temporary base64 buffer afterward, and returns the sleep result.
- `note.h:1977` declares the public SDK API.

## Role In Callers

`NotePayloadSaveAndSleep` relies on `NoteSleep` consuming the command synchronously before the temporary `stateB64` buffer is freed. A false result from `NoteSleep` propagates as the high-level payload-save-and-sleep failure.

## Tests Observed

`test/src/NoteSleep_test.cpp` fakes `NoteNewCommand` and `NoteRequest` to cover command-construction failure skipping `NoteRequest`, request failure returning false, request success returning true, additional modes producing `"sleep,modea,modeb"`, and null modes still calling `NoteRequest`. `test/src/NotePayloadSaveAndSleep_test.cpp` fakes `NoteSleep` to cover the direct caller's success and failure propagation.

## Test Coverage Notes

Focused tests do not assert the `"payload"` string-reference field, the `"seconds"` field, truncation of long modes, unchecked field-add failures, or the unconditional final error log.

## Graph Links

- Graph node id: `function:n_helpers.c:1754:NoteSleep`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
