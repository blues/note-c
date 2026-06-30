# `_n_ptoa32`

## Status

Finalized. Source-inspected pointer-to-hex debug helper.

## Identity

- Kind: function
- Definition: `n_hooks.c:561`
- Declaration/interface: local static-equivalent `NOTE_C_STATIC` inside `#if NOTE_C_SHOW_MALLOC`
- Signature/API shape: `void _n_ptoa32(void *ptr, char *str)`
- Visibility: compiled only when `NOTE_C_SHOW_MALLOC` and not `NOTE_C_LOW_MEM`
- Public/exported names: none
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:544:_n_htoa32` finalized
- External/system dependencies: pointer cast to `uint32_t`
- Data/type/global dependencies: writes caller-provided output buffer

## Behavior

Casts a pointer to `uint32_t` and formats it as eight uppercase hexadecimal characters through `_n_htoa32`.

## Important Boundaries

- Preconditions and assumptions: compiled behind `NOTE_C_SHOW_MALLOC`; a `static_assert` immediately above requires `sizeof(void *) == sizeof(uint32_t)`.
- Error/null/empty behavior: no null or bounds checks; a null pointer formats as zero.
- Ownership and lifetime: writes caller memory and allocates nothing.
- Concurrency/global state: no global state.
- Portability/platform constraints: debug-only and pointer-width constrained by compile-time assertion.
- Performance shape: fixed O(1).

## Direct Call Sites

Exact source search found:

- `n_hooks.c:587` in `NoteMalloc` debug logging.
- `n_hooks.c:603` in `NoteFree` debug logging.

## Role In Callers

Allocator debug logging uses `_n_ptoa32` to display allocated/freed pointer values when `NOTE_C_SHOW_MALLOC && !NOTE_C_LOW_MEM`.

## Tests Observed

None. No focused direct test was inspected for this helper.

## Graph Links

- Graph node id: `function:n_hooks.c:561:_n_ptoa32`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_n_htoa32`.

## Evidence Gaps

No unresolved source gaps for this node.
