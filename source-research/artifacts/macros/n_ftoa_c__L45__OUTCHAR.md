# `OUTCHAR`

## Status

Finalized. Source-inspected bounded output macro for the formatter.

## Identity

- Kind: macro
- Definition: `n_ftoa.c:45`
- Declaration/interface: file-local macro
- Signature/API shape: `OUTCHAR(str, len, size, ch)`
- Visibility: internal to `n_ftoa.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: mutates caller-provided output buffer and length counter

## Behavior

Writes `ch` to `str[len]` only when `len + 1 < size`, preserving room for a later terminator, and increments `len` regardless of whether a byte was written.

## Important Boundaries

- Preconditions and assumptions: caller supplies a writable buffer and an lvalue length expression.
- Error/null/empty behavior: no null checks or error channel.
- Ownership and lifetime: no allocation; writes into caller-owned storage.
- Concurrency/global state: no global state.
- Performance shape: O(1), may evaluate arguments with macro semantics.

## Direct Call Sites

Exact source search found uses in `fmtflt`, `fmtstr`, and `printsep`.

## Role In Callers

The formatter helpers use `OUTCHAR` as their single byte-emission primitive while counting logical output length.

## Tests Observed

Focused JSON number handling and weak direct `JNtoA` tests exercise this indirectly.

## Graph Links

- Graph node id: `macro:n_ftoa.c:45:OUTCHAR`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
