# `printsep`

## Status

Finalized. Source-inspected grouping separator emitter.

## Identity

- Kind: function
- Definition: `n_ftoa.c:415`
- Declaration/interface: file-local prototype at `n_ftoa.c:38`
- Signature/API shape: `static void printsep(char *str, size_t *len, size_t size)`
- Visibility: file-local
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_ftoa.c:45:OUTCHAR` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes caller-provided output buffer and length counter

## Behavior

Emits a comma through `OUTCHAR`.

## Important Boundaries

- Preconditions and assumptions: caller supplies valid formatter output state.
- Error/null/empty behavior: no error channel.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(1).

## Direct Call Sites

Exact source search found `fmtflt` as the production caller, only when grouping separators are active.

## Role In Callers

`fmtflt` uses `printsep` while emitting integer digits when `PRINT_F_QUOTE` grouping is set. `JNtoA` does not set grouping flags, so this path is inactive for current JSON printing.

## Tests Observed

No focused grouping test was inspected; JSON number tests do not exercise grouping.

## Graph Links

- Graph node id: `function:n_ftoa.c:415:printsep`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
