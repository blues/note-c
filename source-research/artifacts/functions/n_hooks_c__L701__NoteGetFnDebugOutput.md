# `NoteGetFnDebugOutput`

## Status

Finalized. Source-inspected public debug-output hook getter.

## Identity

- Kind: function
- Definition: `n_hooks.c:701`
- Declaration/interface: `note.h:539`
- Public/exported names: `NoteGetFnDebugOutput`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteGetFnDebugOutput(debugOutputFn *fn)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: copies the current debug output hook pointer into caller-provided storage

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `hookDebugOutput`
- Artifact coverage: source-inspected artifact

## Behavior

If `fn` is non-null, writes the current `hookDebugOutput` pointer to `*fn`. A null output pointer is ignored. The getter does not lock.

## Important Boundaries

- Preconditions and assumptions: non-null `fn` must point to writable storage for a `debugOutputFn`.
- Error/null/empty behavior: null output pointer is accepted and skipped; no return value or error path.
- Ownership and lifetime: copied callback remains owned by the caller/platform that installed it.
- Concurrency/global state: unsynchronized read of global `hookDebugOutput`.
- Portability/platform constraints: debug output behavior also depends on compile-time debug settings and log-level paths.
- Performance shape: one null check and optional pointer copy.
- Security or parsing constraints: no validation that the copied callback remains valid after return.

## Direct Call Sites

Source/header exact search found only the public declaration in `note.h:539`, the definition in `n_hooks.c:701`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public hook-introspection API for downstream callers that need the currently configured debug output callback.

## Tests Observed

Focused tests in `test/src/NoteGetFnDebugOutput_test.cpp` cover null output pointer and round-trip after `NoteSetFnDebugOutput`. Gap: default/cleared null state is not directly asserted by this getter test.

## Graph Links

- Graph node id: `function:n_hooks.c:701:NoteGetFnDebugOutput`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
