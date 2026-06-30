# `daysByMonth`

## Status

Finalized. Source-inspected calendar month-offset macro.

## Identity

- Kind: macro
- Definition: `n_helpers.c:71`
- Declaration/interface: private macro in `n_helpers.c`
- Signature/API shape: `daysByMonth(y)`
- Visibility: file-local macro
- Public/exported names: none
- Macro aliases/wrappers: none
- Ownership/lifetime role: selects static cumulative-day table

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads static tables `normalYearDaysByMonth` and `leapYearDaysByMonth`
- Artifact coverage: generated artifact exists

## Behavior

Expands to `normalYearDaysByMonth` when `(y & 03)` is nonzero or `y == 0`; otherwise expands to `leapYearDaysByMonth`. The tables contain cumulative days at the start of months for normal and leap years.

## Important Boundaries

- Preconditions and assumptions: `y` is the internal year offset used by the local-time conversion logic, not a full calendar year.
- Error/null/empty behavior: `y == 0` is forced to the normal-year table even though `0 & 03` is zero.
- Ownership and lifetime: returns pointers to static arrays; callers must not free or mutate them.
- Concurrency/global state: reads static arrays only.
- Portability/platform constraints: uses bitwise leap approximation already baked into this date conversion algorithm.
- Performance shape: O(1).
- Security or parsing constraints: none.

## Direct Call Sites

Exact source search found direct callers:

- `n_helpers.c:1077` `NoteLocalTimeST` maps day-of-year to month/day.
- `n_helpers.c:1127` `_yToDays` adds the year-start month offset, which is currently table index 0.

## Role In Callers

This macro supplies the calendar month lookup table for local-time conversion.

## Tests Observed

Focused `test/src/NoteLocalTimeST_test.cpp` covers a normal 2023 local-time conversion through this macro. No direct leap-boundary test was found.

## Graph Links

- Graph node id: `macro:n_helpers.c:71:daysByMonth`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node. Leap-boundary behavior is not directly tested.
