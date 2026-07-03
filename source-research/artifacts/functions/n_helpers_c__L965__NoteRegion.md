# `NoteRegion`

## Status

Finalized. Source-inspected public cached region/timezone accessor.

## Identity

- Kind: function
- Definition: `n_helpers.c:965`
- Declaration/interface: `note.h:1756`
- Signature/API shape: `bool NoteRegion(char **retCountry, char **retArea, char **retZone, int *retZoneOffset)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NoteRegion`
- Macro aliases/wrappers: none
- Ownership/lifetime role: returns borrowed pointers to static region cache or static fallback strings

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:871:NoteTimeST` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: reads `zoneStillUnavailable`, `curCountry`, `curArea`, `curZone`, and `curZoneOffsetMins`
- Artifact coverage: generated artifact exists

## Behavior

Calls `NoteTimeST()` first to refresh cached time/zone state. If `zoneStillUnavailable` remains true, writes fallback values to non-null outputs (`country=""`, `area=""`, `zone="UTC"`, offset `0`) and returns false. Otherwise writes pointers to the cached country/area/zone buffers and cached offset to non-null outputs, then returns true.

## Important Boundaries

- Preconditions and assumptions: output pointers are optional; caller must treat returned strings as borrowed.
- Error/null/empty behavior: null output pointers are ignored; unavailable zone reports false with safe fallback values for provided outputs.
- Ownership and lifetime: returned cached pointers remain owned by module globals/static strings.
- Concurrency/global state: reads global region cache without locking after `NoteTimeST` may mutate it.
- Portability/platform constraints: region cache is populated by Notecard `card.time` response parsing in `NoteTimeST`.
- Performance shape: O(1) plus any refresh transaction performed by `NoteTimeST`.
- Security or parsing constraints: no validation beyond the cached state maintained by `NoteTimeST`.

## Direct Call Sites

Exact source search found direct production caller:

- `n_helpers.c:1056` `NoteLocalTimeST` obtains current zone and offset.

## Role In Callers

This is the public region/timezone cache accessor and local-time dependency.

## Tests Observed

Focused `test/src/NoteRegion_test.cpp` covers unavailable and valid cached region paths, with null and non-null output pointers. `NoteTimeSet_test.cpp` also covers manual region setup and reset behavior through this accessor.

## Graph Links

- Graph node id: `function:n_helpers.c:965:NoteRegion`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `NoteTimeST`.

## Evidence Gaps

No unresolved source gaps for this node.
