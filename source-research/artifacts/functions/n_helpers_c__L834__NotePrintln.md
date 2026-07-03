# `NotePrintln`

## Status

Finalized. Source-inspected public newline-print wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:834`
- Declaration/interface: `note.h:1125`
- Signature/API shape: `void NotePrintln(const char *line)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NotePrintln`
- Macro aliases/wrappers: none
- Ownership/lifetime role: routes caller text and SDK newline constant through `NotePrint`

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:847:NotePrint` finalized in this slice
- External/system dependencies: none directly
- Data/type/global dependencies: reads `c_newline`
- Artifact coverage: generated artifact exists

## Behavior

Calls `NotePrint(line)` and then `NotePrint(c_newline)`. It returns `void` and ignores both `NotePrint` return values.

## Important Boundaries

- Preconditions and assumptions: caller normally passes a non-null C string; null handling follows `NotePrint`.
- Error/null/empty behavior: failures from printing the line or newline are not surfaced.
- Ownership and lifetime: does not retain `line`; no allocation of its own.
- Concurrency/global state: inherits debug hook and request/transport behavior from `NotePrint`.
- Portability/platform constraints: inherits `NOTE_NODEBUG` behavior from `NotePrint`.
- Performance shape: two `NotePrint` calls.
- Security or parsing constraints: no validation of `line`.

## Direct Call Sites

No in-repository production call sites were found beyond the definition and public declaration in `note.h:1125`.

## Role In Callers

This is the public line-oriented wrapper over `NotePrint`.

## Tests Observed

Focused `test/src/NotePrintln_test.cpp` fakes `NotePrint` and verifies the line text is followed by `c_newline`.

## Graph Links

- Graph node id: `function:n_helpers.c:834:NotePrintln`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `NotePrint`.

## Evidence Gaps

No unresolved source gaps for this node.
