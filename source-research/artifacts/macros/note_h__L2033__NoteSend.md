# `NoteSend`

## Status

Finalized. Source-inspected public macro alias.

## Identity

- Kind: macro
- Definition: `note.h:2033`
- Declaration/interface: public header macro in `note.h`
- Public/exported names: `NoteSend`
- Macro aliases/wrappers: transparent alias for `NoteAdd`
- Signature/API shape: macro replacement `#define NoteSend NoteAdd`; callers use the `NoteAdd(const char *target, J *body, bool urgent)` API shape after preprocessing
- Visibility: public note-c header macro
- Ownership/lifetime role: inherits `NoteAdd` body consumption/freeing semantics

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:2083:NoteAdd` finalized
- External/system dependencies: inherited from `NoteAdd`
- Data/type/global dependencies: inherited from `NoteAdd`
- Artifact coverage: source-inspected artifact

## Behavior

Transparent preprocessor alias: `NoteSend` expands directly to `NoteAdd`. It adds no arguments, drops no arguments, performs no ownership conversion, and has no runtime body of its own.

## Important Boundaries

- Preconditions and assumptions: identical to finalized `NoteAdd` after macro expansion.
- Error/null/empty behavior: identical to finalized `NoteAdd`.
- Ownership and lifetime: identical to finalized `NoteAdd`; caller-supplied body is consumed/freed through the `NoteAdd` path.
- Concurrency/global state: no macro-local state; runtime effects are inherited from `NoteAdd`.
- Portability/platform constraints: standard object-like C preprocessor alias in the public header.
- Performance shape: no runtime overhead beyond `NoteAdd`.
- Security or parsing constraints: no macro-local validation or parsing.

## Direct Call Sites

Exact source search found only the macro definition in `note.h:2033`. No production repository caller or focused test using `NoteSend` was found.

## Role In Callers

No production repository caller role. Public compatibility/convenience alias for downstream callers that use `NoteSend` instead of `NoteAdd`.

## Tests Observed

No focused tests call the `NoteSend` macro name directly. `test/src/NoteAdd_test.cpp` covers the target implementation reached after expansion.

## Graph Links

- Graph node id: `macro:note.h:2033:NoteSend`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing `expands_to` edge to `function:n_helpers.c:2083:NoteAdd`.

## Evidence Gaps

No unresolved source gaps for this node.
