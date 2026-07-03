# `NoteLocalTimeST`

## Status

Finalized. Source-inspected public session-local-time conversion helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1016`
- Declaration/interface: `note.h:1745`
- Signature/API shape: `bool NoteLocalTimeST(uint16_t *retYear, uint8_t *retMonth, uint8_t *retDay, uint8_t *retHour, uint8_t *retMinute, uint8_t *retSecond, char **retWeekday, char **retZone)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NoteLocalTimeST`
- Macro aliases/wrappers: none
- Ownership/lifetime role: writes optional output fields and returns borrowed weekday/zone strings

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:755:NoteTimeValidST` finalized
  - `function:n_helpers.c:871:NoteTimeST` finalized
  - `function:n_helpers.c:965:NoteRegion` finalized in this slice
  - `function:n_helpers.c:1119:_yToDays` finalized in this slice
  - `macro:n_helpers.c:71:daysByMonth` finalized in this slice
- External/system dependencies: none directly
- Data/type/global dependencies: reads `dayNames`, mutates static `lastHour`/`lastInfoIsKnown`, and may set global `zoneForceRefresh`
- Artifact coverage: generated artifact exists

## Behavior

Initializes all provided outputs to zero or empty-string defaults. If `NoteTimeValidST()` is false, returns false with defaults. Otherwise reads current epoch time from `NoteTimeST()`, obtains region zone/offset from `NoteRegion`, applies the timezone offset in seconds, and converts Unix epoch seconds into year/month/day/hour/minute/second/weekday using `_yToDays`, `daysByMonth`, and `dayNames`.

Writes the borrowed `currentZone` pointer when requested. It tracks the last local hour in static state and, when region info is available, sets `zoneForceRefresh` if the local hour crosses into 1, 2, or 3 so DST offset changes can be rechecked.

## Important Boundaries

- Preconditions and assumptions: output pointers are optional; returned weekday and zone pointers are borrowed static/cache strings.
- Error/null/empty behavior: invalid time returns false after clearing outputs; unavailable region can still produce UTC-based local fields but returns false because `regionAvailable` is false.
- Ownership and lifetime: no allocation; all outputs are scalar writes or borrowed pointers.
- Concurrency/global state: reads/mutates time/region globals and static last-hour state without locking.
- Portability/platform constraints: conversion uses 32-bit `uint32_t secs` after adding a 1900 offset, so behavior is constrained by that arithmetic.
- Performance shape: bounded loops over year adjustment and 12-month lookup.
- Security or parsing constraints: no validation of offset range; trusts cached region state.

## Direct Call Sites

No in-repository production call sites were found beyond the definition and public declaration in `note.h:1745`.

## Role In Callers

This is the public local date/time conversion API above cached time and region state.

## Tests Observed

Focused `test/src/NoteLocalTimeST_test.cpp` covers invalid time defaulting and a valid 2023 PDT conversion with weekday, zone, and time fields. Leap-boundary and DST-refresh-trigger cases were not found.

## Graph Links

- Graph node id: `function:n_helpers.c:1016:NoteLocalTimeST`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteTimeValidST`, `NoteTimeST`, `NoteRegion`, `_yToDays`, and `daysByMonth`.

## Evidence Gaps

No unresolved source gaps for this node. Leap-boundary and DST-refresh-trigger behavior are not directly tested.
