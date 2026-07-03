# `_LockNote`

## Status

Finalized. Macro definition, expansion target, target implementation, and production call sites were inspected from source.

## Identity

- Kind: macro
- Definition: `n_lib.h:226`
- Declaration/interface: internal wrapper macro in `n_lib.h`.
- Public/exported names: none.
- Macro aliases/wrappers: expands directly to `_noteLockNote`.
- Signature/API shape: object-like macro used as a no-argument function call, `_LockNote()`.
- Visibility: project-internal to translation units including `n_lib.h`.
- Ownership/lifetime role: provides the canonical internal spelling for Notecard-wide lock acquisition; no allocation or ownership transfer.

## Dependency Status

- Project-local dependencies:
- `function:n_hooks.c:657:_noteLockNote` (expands_to, medium, verified=False)
- External/system dependencies: none directly.
- Data/type/global dependencies: through `_noteLockNote`, reads the `hookLockNote` global function pointer and dispatches a platform `mutexFn` when set.
- Artifact coverage: `_noteLockNote` target artifact finalized at `artifacts/functions/n_hooks_c__L657__noteLockNote.md`.

## Behavior

`_LockNote` is a readability macro. The preprocessor replaces `_LockNote` with `_noteLockNote`, so every `_LockNote()` call executes the hook-dispatch function implemented at `n_hooks.c:657`. If the platform has registered a note mutex hook, that hook is invoked; otherwise locking is a no-op.

## Important Boundaries

- Preconditions and assumptions: callers use this before touching Notecard transport or global hook state that should be serialized.
- Error/null/empty behavior: no error path; absent hook means no-op.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: the macro participates in optional platform-provided synchronization; if no hook is registered, callers are not serialized by note-c itself.
- Portability/platform constraints: portable macro over a function-pointer hook dispatch.
- Performance shape: one macro expansion plus O(1) function-pointer check in `_noteLockNote`.
- Security or parsing constraints: none directly.

## Direct Call Sites

Exact production search `rg -n "\b_LockNote\b" n_hooks.c n_request.c n_helpers.c n_lib.h` found:

- Definition: `n_lib.h:226`.
- Hook/global-state setters: `n_hooks.c:243`, `n_hooks.c:250`, `n_hooks.c:269`, `n_hooks.c:287`, `n_hooks.c:296`, `n_hooks.c:315`, `n_hooks.c:344`, `n_hooks.c:359`, `n_hooks.c:385`, `n_hooks.c:403`, `n_hooks.c:436`.
- Hook/global-state getters: `n_hooks.c:727`, `n_hooks.c:777`, `n_hooks.c:796`, `n_hooks.c:816`.
- Request/transaction paths: `n_request.c:295`, `n_request.c:527`, `n_request.c:767`, `n_request.c:892`.
- Binary helper paths: `n_helpers.c:380`, `n_helpers.c:639`.

## Role In Callers

- Hook APIs use `_LockNote` to protect active interface, default hook installation, serial/I2C hook installation, transaction hook configuration, debug hook configuration, and hook reads.
- Request APIs use `_LockNote` to serialize JSON transaction execution, reset, and ping.
- Binary helper APIs use `_LockNote` before issuing binary get/put transactions and then call `_noteTransactionShouldLock(..., false)` to avoid taking the same note lock again.

## Tests Observed

None. Tests were not needed to verify this internal alias and its production call sites.

## Graph Links

- Graph node id: `macro:n_lib.h:226:_LockNote`
- Structured graph: `inventory/dependency-graph.json`
- Callee artifacts: `artifacts/functions/n_hooks_c__L657__noteLockNote.md`.
- Caller artifact candidates: hook setter/getter nodes in `n_hooks.c`, request transaction nodes in `n_request.c`, and binary helper nodes in `n_helpers.c`.
- Upstream public behavior candidates: `function:n_request.c:220:NoteRequestResponse`, `function:n_request.c:766:NoteReset`, `function:n_hooks.c:241:NoteSetActiveInterface`, and binary helper APIs in `n_helpers.c`.

## Evidence Gaps

The alias is finalized, but many callers remain preliminary. Dynamic target of the platform mutex hook remains unresolved by design; it is supplied by embedding code outside this repository.
