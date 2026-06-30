# `NoteFree`

## Status

Finalized. Source-inspected platform-hook free wrapper.

## Identity

- Kind: function
- Definition: `n_hooks.c:595`
- Declaration/interface: `note.h:1057`
- Signature/API shape: `void NoteFree(void *ptr)`
- Visibility: public API declaration, implemented in `n_hooks.c`
- Public/exported names: `NoteFree`
- Macro aliases/wrappers: `_Free` in `n_lib.h:242`

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:308:_noteIsDebugOutputActive` finalized
  - `function:n_hooks.c:561:_n_ptoa32` finalized
- External/system dependencies: platform hook `hookFree`
- Data/type/global dependencies: reads static `hookFree`; optionally reads `hookDebugOutput` through debug predicate

## Behavior

If `hookFree` is non-NULL, optionally writes a `"free "` debug line with the pointer value when `NOTE_C_SHOW_MALLOC && !NOTE_C_LOW_MEM` and debug output is active, then calls `hookFree(ptr)`. If `hookFree` is NULL, returns without doing anything.

## Important Boundaries

- Preconditions and assumptions: free behavior is supplied by the registered platform hook.
- Error/null/empty behavior: missing free hook is a silent no-op. A null `ptr` is passed to `hookFree` if a hook exists.
- Ownership and lifetime: releases caller-provided allocation according to the registered hook's contract.
- Concurrency/global state: reads hook globals without locking.
- Portability/platform constraints: deallocation stays behind the SDK hook API; debug pointer printing is low-memory excluded and pointer-width constrained.
- Performance shape: one hook call plus optional debug formatting.

## Direct Call Sites

Exact source search found no production direct calls to `NoteFree` except the `_Free` alias and implementation/declaration. `_Free` fanout covers JSON cleanup, request buffers, transport buffers, payload buffers, and memory helpers.

## Role In Callers

`_Free` aliases to `NoteFree` so internal modules can deallocate through the registered platform hook.

## Tests Observed

None. No focused direct `NoteFree` test was inspected.

## Graph Links

- Graph node id: `function:n_hooks.c:595:NoteFree`
- Structured graph: `inventory/dependency-graph.json`
- Verified alias edge from `macro:n_lib.h:242:_Free`.

## Evidence Gaps

No unresolved source gaps for this node.
