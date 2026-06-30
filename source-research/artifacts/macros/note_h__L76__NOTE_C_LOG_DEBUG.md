# `NOTE_C_LOG_DEBUG`

## Status

Finalized. Source-inspected low-memory debug log no-op macro.

## Identity

- Kind: macro
- Definition: `note.h:76`
- Declaration/interface: low-memory conditional macro
- Signature/API shape: `NOTE_C_LOG_DEBUG(msg)`
- Visibility: public logging macro in `NOTE_C_LOW_MEM` builds
- Public/exported names: `NOTE_C_LOG_DEBUG`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `NOTE_C_LOW_MEM`

## Behavior

When `NOTE_C_LOW_MEM` is defined, expands to an empty `do { } while (0)` block.

## Important Boundaries

- Preconditions and assumptions: message expression is not evaluated in this variant.
- Error/null/empty behavior: all debug log output is suppressed.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(1) no-op.
- Portability/platform constraints: selected by low-memory compile mode.

## Direct Call Sites

Exact source search found debug logging throughout request, helper, and hook code.

## Role In Callers

Low-memory transaction builds compile `_noteTransactionShouldLock` debug logs away through this macro.

## Tests Observed

Low-memory syntax checks and debug tests provide indirect evidence; normal debug behavior is covered by the non-low-memory macro artifact.

## Graph Links

- Graph node id: `macro:note.h:76:NOTE_C_LOG_DEBUG`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
