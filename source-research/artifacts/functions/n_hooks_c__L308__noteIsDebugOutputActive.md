# `_noteIsDebugOutputActive`

## Status

Finalized. Source-inspected debug-output hook predicate.

## Identity

- Kind: function
- Definition: `n_hooks.c:308`
- Declaration/interface: `n_lib.h:152`
- Signature/API shape: `bool _noteIsDebugOutputActive(void)`
- Visibility: internal/private helper declared in `n_lib.h`
- Public/exported names: none in `note.h`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads static hook pointer `hookDebugOutput` declared in `n_hooks.c`

## Behavior

Returns `true` when `hookDebugOutput` is non-NULL and `false` otherwise.

## Important Boundaries

- Preconditions and assumptions: none.
- Error/null/empty behavior: null hook means inactive debug output.
- Ownership and lifetime: reads hook pointer only.
- Concurrency/global state: observes global hook state without locking; callers use it as an advisory debug predicate.
- Performance shape: O(1).

## Direct Call Sites

Exact source search found production callers:

- `n_helpers.c:847` in `NotePrint`.
- `n_hooks.c:474` in `NoteDebug`.
- `n_hooks.c:576` and `n_hooks.c:599` in allocator debug logging.
- `n_printf.c:35` in `NoteDebugf`.

## Role In Callers

In the active allocator slice, `NoteMalloc` and `NoteFree` call this predicate only inside `NOTE_C_SHOW_MALLOC && !NOTE_C_LOW_MEM` debug logging blocks before writing allocation/free messages.

## Tests Observed

None. No focused direct test was inspected for this helper.

## Graph Links

- Graph node id: `function:n_hooks.c:308:_noteIsDebugOutputActive`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
