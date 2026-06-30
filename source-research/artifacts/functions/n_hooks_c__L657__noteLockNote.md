# `_noteLockNote`

## Status

Finalized. Implementation, internal declaration, hook-pointer state, macro alias, production call sites, and focused negative direct-symbol search were inspected from source.

## Identity

- Kind: function
- Definition: `n_hooks.c:657`
- Declaration/interface: `n_lib.h:136`
- Public/exported names: none; this is an internal hook-dispatch function.
- Macro aliases/wrappers: `_LockNote` expands to `_noteLockNote` at `n_lib.h:226`.
- Signature/API shape: `void _noteLockNote(void)`
- Visibility: project-internal through `n_lib.h`; used by production C files through `_LockNote`.
- Ownership/lifetime role: does not allocate, free, or transfer ownership; delegates synchronization to a registered platform mutex hook.

## Dependency Status

- Project-local dependencies:
- No project-local function callees. The function dispatches through the `hookLockNote` function pointer when it is non-NULL.
- External/system dependencies: none directly; runtime behavior depends on the platform-provided `mutexFn` registered through `NoteSetFnMutex` or `NoteSetFnNoteMutex`.
- Data/type/global dependencies: reads global `hookLockNote` declared at `n_hooks.c:63`; type is `mutexFn` from `note.h:230`.
- Artifact coverage: this artifact covers the dispatch function; `_LockNote` alias and setter/getter nodes remain separate graph nodes.

## Behavior

1. Reads the global `hookLockNote` pointer.
2. If `hookLockNote != NULL`, calls the registered platform mutex function with no arguments and no return value.
3. If no hook is registered, returns immediately with no locking and no error signal.

## Important Boundaries

- Preconditions and assumptions: callers assume any required Notecard-wide mutual exclusion has been registered by the embedding platform.
- Error/null/empty behavior: `NULL` hook means no-op; the function has no way to report lock failure.
- Ownership and lifetime: no ownership transfer; registered function pointer lifetime is owned by the embedding platform.
- Concurrency/global state: reads unsynchronized global `hookLockNote`; setter APIs write the same global. Locking is advisory and platform-provided.
- Portability/platform constraints: portable C function-pointer hook; behavior depends on platform adapter implementation.
- Performance shape: O(1), one branch and possible indirect function call.
- Security or parsing constraints: none directly.

## Direct Call Sites

Direct symbol search:

- `rg -n "\b_noteLockNote\b" n_hooks.c n_lib.h note.h n_request.c n_helpers.c` found only the internal declaration (`n_lib.h:136`), macro alias (`n_lib.h:226`), and definition (`n_hooks.c:657`).

Macro-alias production call sites through `_LockNote`:

- Hook/global-state setters: `n_hooks.c:243`, `n_hooks.c:250`, `n_hooks.c:269`, `n_hooks.c:287`, `n_hooks.c:296`, `n_hooks.c:315`, `n_hooks.c:344`, `n_hooks.c:359`, `n_hooks.c:385`, `n_hooks.c:403`, `n_hooks.c:436`.
- Hook/global-state getters: `n_hooks.c:727`, `n_hooks.c:777`, `n_hooks.c:796`, `n_hooks.c:816`.
- Request/transaction paths: `n_request.c:295`, `n_request.c:527`, `n_request.c:767`, `n_request.c:892`.
- Binary helper paths: `n_helpers.c:380`, `n_helpers.c:639`.

## Role In Callers

- `_LockNote` exposes this dispatch function as the conventional internal spelling used by the rest of the SDK.
- Request paths use it to serialize Notecard access around JSON transactions, reset, ping, and binary transfer sequences.
- Hook setter/getter paths use it to protect mutation or reads of global hook state, except `NoteSetFnMutex`, `NoteSetFnI2CMutex`, `NoteSetFnNoteMutex`, and note-mutex getters, which directly write/read mutex hook pointers without taking the note lock they may be changing.

## Tests Observed

None. Tests were not needed to establish the source behavior of this hook dispatch node.

## Graph Links

- Graph node id: `function:n_hooks.c:657:_noteLockNote`
- Structured graph: `inventory/dependency-graph.json`
- Callee artifacts: none for direct project-local functions; indirect platform callback target is represented as hook-pointer behavior rather than a concrete repository callee.
- Caller artifact candidates: `artifacts/macros/n_lib_h__L226__LockNote.md`, request-path callers, hook setter/getter callers, and binary helper callers listed above.
- Upstream public behavior candidates: `function:n_request.c:220:NoteRequestResponse`, `function:n_request.c:766:NoteReset`, `function:n_hooks.c:241:NoteSetActiveInterface`, hook registration APIs in `n_hooks.c`, and binary helper APIs in `n_helpers.c`.

## Evidence Gaps

No focused tests were inspected. Source inspection found dynamic dispatch through `hookLockNote`; the exact runtime target is platform-provided and cannot be statically resolved inside this repository.
