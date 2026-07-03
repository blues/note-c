# `convert`

## Status

Finalized. Source-inspected reverse digit conversion helper.

## Identity

- Kind: function
- Definition: `n_ftoa.c:448`
- Declaration/interface: file-local prototype at `n_ftoa.c:41`
- Signature/API shape: `static int convert(uintmax_t value, char *buf, size_t size, int base, int caps)`
- Visibility: file-local
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes caller-provided buffer

## Behavior

Writes digits for `value` into `buf` in reverse order, using lowercase or uppercase digits according to `caps`, stopping when the value becomes zero or `pos == size`. Returns the number of bytes written. The result is not NUL-terminated.

## Important Boundaries

- Preconditions and assumptions: caller supplies a nonzero buffer size and a valid base for the digit table.
- Error/null/empty behavior: no null checks or error channel.
- Ownership and lifetime: no allocation; writes caller-owned scratch storage.
- Concurrency/global state: no global state.
- Performance shape: O(number of emitted digits).

## Direct Call Sites

Exact source search found `fmtflt` callers for exponent, integer, and fractional digit conversion.

## Role In Callers

`fmtflt` reverses the returned digit runs while emitting integer, fractional, and exponent components.

## Tests Observed

Focused JSON number handling tests exercise this indirectly through `JPrintUnformatted`.

## Graph Links

- Graph node id: `function:n_ftoa.c:448:convert`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
