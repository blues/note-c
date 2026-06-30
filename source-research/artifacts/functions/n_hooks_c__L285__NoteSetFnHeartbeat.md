# `NoteSetFnHeartbeat`

## Status

Finalized. Source-inspected conditional public heartbeat hook setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:285`
- Declaration/interface: `note.h:547`
- Public/exported names: `NoteSetFnHeartbeat` when `NOTE_C_HEARTBEAT_CALLBACK` is enabled
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFnHeartbeat(heartbeatFn fn, void *context)`
- Visibility: conditional public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: stores a caller-provided heartbeat callback pointer and opaque context

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes `hookHeartbeat` and `hookHeartbeatContext`
- Artifact coverage: source-inspected artifact

## Behavior

When compiled with `NOTE_C_HEARTBEAT_CALLBACK`, locks Notecard access, assigns the heartbeat function and context globals from the supplied arguments, and unlocks. Null `fn` disables heartbeat dispatch; the context value is still stored even when the function pointer is null.

## Important Boundaries

- Preconditions and assumptions: callback and context are accepted as opaque values.
- Error/null/empty behavior: no return value or error path; null callback clears/disables heartbeat dispatch.
- Ownership and lifetime: callback/context ownership stays with the caller; note-c stores raw values.
- Concurrency/global state: updates heartbeat globals under `_LockNote`/`_UnlockNote`.
- Portability/platform constraints: symbol exists only in `NOTE_C_HEARTBEAT_CALLBACK` builds.
- Performance shape: one lock, two assignments, one unlock.
- Security or parsing constraints: no validation that callback/context remain valid after return.

## Direct Call Sites

Source/header exact search found only the conditional public declaration in `note.h:547`, the conditional definition in `n_hooks.c:285`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public API for downstream adapter code that wants transaction heartbeat callbacks in heartbeat-enabled builds.

## Tests Observed

Focused tests in `test/src/NoteSetFnHeartbeat_test.cpp`, `test/src/NoteGetFnHeartbeat_test.cpp`, `test/src/_noteHeartbeat_test.cpp`, and heartbeat-enabled transaction tests cover setter/getter/dispatch behavior. Gaps include every null-argument branch's lock pairing and the symbol's absence in non-heartbeat builds.

## Graph Links

- Graph node id: `function:n_hooks.c:285:NoteSetFnHeartbeat`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockNote` and `_UnlockNote`.

## Evidence Gaps

No unresolved source gaps for this node.
