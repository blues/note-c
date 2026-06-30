# `NotePrint`

## Status

Finalized. Source-inspected public debug/card.log print helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:847`
- Declaration/interface: `note.h:1119`
- Signature/API shape: `bool NotePrint(const char *text)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NotePrint`
- Macro aliases/wrappers: none
- Ownership/lifetime role: routes text to active debug hook or transfers a `card.log` request to `NoteRequest`

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:308:_noteIsDebugOutputActive` finalized
  - `macro:n_lib.h:256:_Debug` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: reads debug hook state and may send `card.log`
- Artifact coverage: generated artifact exists

## Behavior

If debug output is active, calls `_Debug(text)` and returns `true` without sending a Notecard request. In `NOTE_NODEBUG` builds `_Debug` expands to nothing, so this path still reports success when the debug hook is active.

When debug output is inactive, creates a `card.log` request, adds `"text": text`, and returns `NoteRequest(req)`. The return from `JAddStringToObject` is ignored; request allocation failure flows through `JAddStringToObject(NULL, ...)` and `NoteRequest(NULL)` to `false`.

## Important Boundaries

- Preconditions and assumptions: callers normally pass a non-null C string; null text is passed through to `_Debug` on the debug path or causes `JAddStringToObject` to return `NULL` on the card.log path.
- Error/null/empty behavior: debug-active path always returns true after `_Debug`; card.log path returns the boolean from `NoteRequest`.
- Ownership and lifetime: does not retain `text`; `NoteRequest` consumes the request object when one is created.
- Concurrency/global state: observes debug hook state without locking and otherwise uses request/transport hooks.
- Portability/platform constraints: `_Debug` behavior changes under `NOTE_NODEBUG`.
- Performance shape: O(length of text) for debug/card.log string handling plus one request transaction on the card.log path.
- Security or parsing constraints: no explicit validation of `text`; logging request may be sent without a `"text"` field if string attachment fails.

## Direct Call Sites

Exact source search found direct production callers:

- `n_helpers.c:836` and `n_helpers.c:837` in `NotePrintln`.
- `n_printf.c:67` in `NotePrintf`.

## Role In Callers

This is the shared output primitive for line-oriented and formatted public print helpers.

## Tests Observed

Focused `test/src/NotePrint_test.cpp` covers debug output mode, card.log request allocation failure, `NoteRequest` failure, and `NoteRequest` success. The test accounts for `NOTE_NODEBUG` suppressing `NoteDebug`.

## Graph Links

- Graph node id: `function:n_helpers.c:847:NotePrint`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_noteIsDebugOutputActive`, `_Debug`, `NoteNewRequest`, `JAddStringToObject`, and `NoteRequest`.

## Evidence Gaps

No unresolved source gaps for this node.
