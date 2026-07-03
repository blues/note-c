# `NoteFactoryReset`

## Status

Finalized. Source-inspected public factory-reset helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1902`
- Declaration/interface: `note.h:1994`
- Public/exported names: `NoteFactoryReset`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NoteFactoryReset(bool deleteConfigSettings)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: constructs a `card.restore` request and transfers it to `NoteRequest`; then drives delay/log/reset hooks after a successful request

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:1950:JAddBoolToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
  - `macro:n_lib.h:244:_DelayMs` finalized
  - `macro:note.h:1006:NOTE_C_LOG_INFO` finalized
  - `function:n_request.c:766:NoteReset` finalized
- External/system dependencies: Notecard `card.restore` transaction and post-restore reset path through request/delay/log/reset hooks
- Data/type/global dependencies: transitive request/transaction globals and hooks; `_DelayMs` uses the configured delay hook; `NOTE_C_LOG_INFO` uses configured log hook; `NoteReset` touches request/reset global state through its finalized path
- Artifact coverage: source-inspected artifact

## Behavior

Creates a `card.restore` request. If request allocation succeeds, adds `"delete"` with the `deleteConfigSettings` argument and sends it with `NoteRequest`. If request creation or request execution fails, returns false immediately. After a successful restore request, waits 5000 ms, logs `"CARD RESTORED\n"`, then repeatedly calls `NoteReset()` until it returns true, sleeping 5000 ms between failed reset attempts. Returns true only after `NoteReset()` succeeds.

## Important Boundaries

- Preconditions and assumptions: no pointer inputs. `deleteConfigSettings` is copied into the JSON `"delete"` field.
- Error/null/empty behavior: request allocation failure returns false. `NoteRequest` failure returns false and skips delay/log/reset. `JAddBoolToObject` return value is ignored, so bool-field allocation/attach failure is not directly surfaced before `NoteRequest`.
- Ownership and lifetime: request ownership transfers to `NoteRequest`, which handles request/response cleanup through its finalized path.
- Concurrency/global state: request, logging, delay, and reset behavior use global SDK hook/state layers; there is no local static state in this wrapper.
- Portability/platform constraints: platform timing/logging behavior is behind `_DelayMs` and `NOTE_C_LOG_INFO` hooks.
- Performance shape: one restore request transaction, at least one 5000 ms delay on success, one log call, then one or more `NoteReset` attempts with 5000 ms delay between failures.
- Security or parsing constraints: no timeout or max-attempt limit around the `NoteReset` loop; if `NoteReset` never succeeds, this function does not return.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1994`, the definition in `n_helpers.c:1902`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that restore the Notecard to factory settings and then wait for/reset the card.

## Tests Observed

Focused tests in `test/src/NoteFactoryReset_test.cpp` fake `NoteNewRequest`, `NoteRequest`, and `NoteReset`. Covered cases: request creation failure returns false; request failure returns false; successful request writes `"delete"` true/false as requested; and a first reset failure is retried until success. Gaps include unchecked `JAddBoolToObject` failure, delay/log hook effects, and the infinite-loop case when `NoteReset` never succeeds.

## Graph Links

- Graph node id: `function:n_helpers.c:1902:NoteFactoryReset`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteNewRequest`, `JAddBoolToObject`, `NoteRequest`, `_DelayMs`, `NOTE_C_LOG_INFO`, and `NoteReset`.

## Evidence Gaps

No unresolved source gaps for this node.
