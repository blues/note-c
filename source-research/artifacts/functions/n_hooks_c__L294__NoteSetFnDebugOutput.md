# `NoteSetFnDebugOutput`

## Status

Finalized. Source-inspected public debug-output hook setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:294`
- Declaration/interface: `note.h:533`
- Public/exported names: `NoteSetFnDebugOutput`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetFnDebugOutput(debugOutputFn fn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: stores a caller-provided debug output callback pointer

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:226:_LockNote` finalized
  - `macro:n_lib.h:227:_UnlockNote` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes `hookDebugOutput`
- Artifact coverage: source-inspected artifact

## Behavior

Locks Notecard access, assigns `hookDebugOutput = fn`, and unlocks. Null clears the debug output hook; `_noteIsDebugOutputActive` and debug output helpers then treat debug output as inactive.

## Important Boundaries

- Preconditions and assumptions: callback pointer is accepted as an opaque value.
- Error/null/empty behavior: no return value or error path; null clears the hook.
- Ownership and lifetime: callback ownership stays with the caller/platform; note-c stores the raw function pointer.
- Concurrency/global state: updates `hookDebugOutput` under `_LockNote`/`_UnlockNote`.
- Portability/platform constraints: debug/log output still depends on `NOTE_NODEBUG` and log-level compilation/runtime behavior.
- Performance shape: one lock, one pointer assignment, one unlock.
- Security or parsing constraints: no validation that callback remains valid after return.

## Direct Call Sites

Source/header exact search found only the public declaration in `note.h:533`, the definition in `n_hooks.c:294`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public API for downstream adapter code that wants to route SDK debug/log output.

## Tests Observed

Focused tests in `test/src/NoteSetFnDebugOutput_test.cpp`, `test/src/NoteGetFnDebugOutput_test.cpp`, `test/src/NoteDebug_test.cpp`, and `test/src/NoteDebugf_test.cpp` cover setting/getting and debug output use. Gap: no concurrent set/use coverage.

## Graph Links

- Graph node id: `function:n_hooks.c:294:NoteSetFnDebugOutput`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_LockNote` and `_UnlockNote`.

## Evidence Gaps

No unresolved source gaps for this node.
