# `JItoA`

## Status

Finalized. Source-inspected signed integer to decimal string helper.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:284`
- Declaration/interface: `note.h:1473`
- Signature/API shape: `void JItoA(JINTEGER n, char *s)`
- Visibility: public header declaration, implemented in `n_cjson_helpers.c`
- Public/exported names: `JItoA`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `strlen`
- Data/type/global dependencies: writes caller-provided string buffer; no global state

## Behavior

Converts a signed `JINTEGER` to a decimal C string. Negative values are converted through a `JUINTEGER` temporary so `JINTEGER_MIN` can be negated without signed overflow. Digits are emitted in reverse order, an optional minus sign is appended, the string is NUL-terminated, then the buffer is reversed in place.

## Important Boundaries

- Preconditions and assumptions: caller must provide enough writable buffer space for the decimal representation and NUL terminator.
- Error/null/empty behavior: no null or bounds checks.
- Ownership and lifetime: writes caller memory and allocates nothing.
- Concurrency/global state: no global state.
- Portability/platform constraints: explicitly handles the minimum signed integer case by using unsigned arithmetic.
- Performance shape: O(number of digits).

## Direct Call Sites

Exact source search found production callers:

- `n_cjson.c:412` uses `JItoA` in numeric printing.
- `n_helpers.c:1235` and `n_helpers.c:1281` use it for environment integer helpers.
- `n_hooks.c:453` uses it in `NoteDebugIntln`.
- `n_hooks.c:580` uses it in `NoteMalloc` debug logging.

## Role In Callers

In the active allocator slice, `NoteMalloc` uses `JItoA` only when `NOTE_C_SHOW_MALLOC && !NOTE_C_LOW_MEM` and debug output is active, converting allocation size to printable text.

## Tests Observed

None. No focused direct test was inspected for this helper.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:284:JItoA`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
