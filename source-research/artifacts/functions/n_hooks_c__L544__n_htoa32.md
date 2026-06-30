# `_n_htoa32`

## Status

Finalized. Source-inspected 32-bit uppercase hex helper.

## Identity

- Kind: function
- Definition: `n_hooks.c:544`
- Declaration/interface: `n_lib.h:158`
- Signature/API shape: `void _n_htoa32(uint32_t n, char *p)`
- Visibility: internal/private helper declared in `n_lib.h`; compiled only when `NOTE_C_LOW_MEM` is not defined
- Public/exported names: none in `note.h`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: primitive shifts, masks, comparisons, and byte stores
- Data/type/global dependencies: writes caller-provided output buffer

## Behavior

Converts a 32-bit value to eight uppercase hexadecimal characters by repeatedly taking the high nibble, shifting left four bits, writing `'A'`-based or `'0'`-based characters, and appending a NUL terminator.

## Important Boundaries

- Preconditions and assumptions: caller must provide at least nine writable bytes.
- Error/null/empty behavior: no null or bounds checks.
- Ownership and lifetime: writes caller memory and allocates nothing.
- Concurrency/global state: no global state.
- Portability/platform constraints: excluded in `NOTE_C_LOW_MEM` builds.
- Performance shape: fixed O(1).

## Direct Call Sites

Exact source search found:

- `n_hooks.c:563` called by `_n_ptoa32`.
- `n_request.c:1046` called by `_crcAdd`.

## Role In Callers

In the active allocator slice, `_n_ptoa32` uses `_n_htoa32` to print pointer values when allocation debug logging is compiled.

## Tests Observed

None. No focused direct test was inspected for this helper.

## Graph Links

- Graph node id: `function:n_hooks.c:544:_n_htoa32`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
