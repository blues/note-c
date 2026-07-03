# `NoteTimeSet`

## Status

Finalized. Source-inspected public manual time and region setter.

## Identity

- Kind: function
- Definition: `n_helpers.c:808`
- Declaration/interface: `note.h:1730`
- Public/exported names: `NoteTimeSet`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteTimeSet(JTIME secondsUTC, int offset, char *zone, char *country, char *area)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: copies caller-provided region strings into module-local static buffers and updates cached time state

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:243:_GetMs` finalized
  - `function:n_str.c:31:strlcpy` finalized
- External/system dependencies: platform `strlcpy` may be used when available; the repository provides `n_str.c` when the build lacks it
- Data/type/global dependencies: writes static `timeBaseSec`, `timeBaseSetAtMs`, `timeBaseSetManually`, `zoneStillUnavailable`, `zoneForceRefresh`, `curZoneOffsetMins`, `curZone`, `curArea`, and `curCountry`
- Artifact coverage: source-inspected artifact

## Behavior

When `secondsUTC == 0`, clears the cached manual time by setting `timeBaseSec = 0`, `timeBaseSetAtMs = 0`, `timeBaseSetManually = false`, `zoneStillUnavailable = true`, and `zoneForceRefresh = false`. It does not update the region string buffers in this reset branch; later `NoteRegion` returns UTC and empty-region defaults while `zoneStillUnavailable` remains true.

When `secondsUTC` is nonzero, stores it as `timeBaseSec`, snapshots `_GetMs()` into `timeBaseSetAtMs`, marks the time as manually set, marks zone information available, stores the timezone offset, and copies the provided `zone`, `area`, and `country` strings into static buffers. Null `zone` becomes `"UTC"`; null `area` and `country` become empty strings.

## Important Boundaries

- Preconditions and assumptions: non-null `zone`, `country`, and `area` inputs must be valid C strings when provided.
- Error/null/empty behavior: null region inputs on the nonzero path are accepted and replaced with defaults. There is no return value or error path.
- Ownership and lifetime: input strings are copied, not retained. Returned region data from `NoteRegion` points at static buffers owned by this module.
- Concurrency/global state: no locking around time and region statics; concurrent readers such as `NoteTimeST` and `NoteRegion` can observe unsynchronized updates.
- Portability/platform constraints: `_GetMs` depends on the configured timing hook and returns zero when no hook is set. `strlcpy` availability is build-dependent, with a project fallback when the platform lacks it.
- Performance shape: constant number of scalar assignments plus bounded string copies into `curZone[10]`, `curArea[64]`, and `curCountry[8]`.
- Security or parsing constraints: `strlcpy` truncates long strings silently; no validation of `offset` range or string contents.

## Direct Call Sites

Production-only exact search found only the public declaration in `note.h:1730` and the definition in `n_helpers.c:808`. Focused tests call it directly from `test/src/NoteTimeSet_test.cpp` and `test/src/NoteTime_test.cpp`. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that want to inject host-provided time and region state or clear manual time so later calls fall back to Notecard `card.time`.

## Tests Observed

Focused tests in `test/src/NoteTimeSet_test.cpp` cover setting manual time and region, clearing manual time with `secondsUTC == 0` so later `NoteTime` requests `card.time`, invalidating `NoteRegion`, and null region inputs defaulting to UTC/empty strings. `test/src/NoteTime_test.cpp` uses `NoteTimeSet` to cover millisecond rollover handling in `NoteTime`. Gaps include long-string truncation, offset boundary values, absent `NoteGetMs` hook behavior during manual set, and concurrency/locking.

## Graph Links

- Graph node id: `function:n_helpers.c:808:NoteTimeSet`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_GetMs` and `strlcpy`.

## Evidence Gaps

No unresolved source gaps for this node.
