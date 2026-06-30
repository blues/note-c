# `_yToDays`

## Status

Finalized. Source-inspected internal leap-day offset helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1119`
- Declaration/interface: file-local prototype at `n_helpers.c:79`
- Signature/API shape: `int _yToDays(int year)`
- Visibility: internal to `n_helpers.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none
- Macro aliases/wrappers: none
- Ownership/lifetime role: pure calendar arithmetic helper

## Dependency Status

- Project-local dependencies:
  - `macro:n_helpers.c:71:daysByMonth` finalized in this slice
- External/system dependencies: none
- Data/type/global dependencies: reads static month table index 0 through `daysByMonth(year)[0]`
- Artifact coverage: generated artifact exists

## Behavior

Computes the number of leap-day corrections before the internal year offset. For positive years, `(year - 1) / 4`; for years `<= -4`, `year / 4`; otherwise zero. Returns that value plus `daysByMonth(year)[0]`, which is zero for both month tables.

## Important Boundaries

- Preconditions and assumptions: `year` is the same internal year offset used by `NoteLocalTimeST`.
- Error/null/empty behavior: years from -3 through 0 contribute zero leap-day adjustment.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads static arrays only.
- Portability/platform constraints: uses C integer division for negative years; no Gregorian century rule is represented.
- Performance shape: O(1).
- Security or parsing constraints: none.

## Direct Call Sites

Exact source search found direct caller:

- `n_helpers.c:1070` `NoteLocalTimeST` uses it to locate the start day of the current internal year.

## Role In Callers

This helper supports year selection during epoch-to-local-date conversion.

## Tests Observed

Focused `test/src/NoteLocalTimeST_test.cpp` covers the helper indirectly for a 2023 conversion. No direct tests were found for negative years or leap boundaries.

## Graph Links

- Graph node id: `function:n_helpers.c:1119:_yToDays`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `daysByMonth`.

## Evidence Gaps

No unresolved source gaps for this node. Leap-boundary and negative-year behavior are not directly tested.
