# `fmtstr`

## Status

Finalized. Source-inspected bounded string formatting helper.

## Identity

- Kind: function
- Definition: `n_ftoa.c:378`
- Declaration/interface: file-local prototype at `n_ftoa.c:36`
- Signature/API shape: `static void fmtstr(char *str, size_t *len, size_t size, const char *value, int width, int precision, int flags)`
- Visibility: file-local
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_ftoa.c:45:OUTCHAR` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes caller-provided output buffer and length counter

## Behavior

Formats a string into the shared output state with optional width, precision, and left-justify padding. A null input value is treated as `"(null)"`. It measures up to either NUL or precision, emits leading spaces unless left-justified, emits the string bytes, then emits trailing spaces for left-justify mode.

## Important Boundaries

- Preconditions and assumptions: caller supplies valid formatter output state.
- Error/null/empty behavior: null `value` prints `"(null)"`.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(output bytes counted/emitted).

## Direct Call Sites

Exact source search found `fmtflt` as the production caller for direct NaN/Infinity text formatting.

## Role In Callers

`fmtflt` uses `fmtstr` only for direct `JNtoA` NaN/Infinity output. JSON number printing bypasses `JNtoA` for non-finite values and emits `null`.

## Tests Observed

No focused NaN/Infinity `JNtoA` test was inspected.

## Graph Links

- Graph node id: `function:n_ftoa.c:378:fmtstr`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
