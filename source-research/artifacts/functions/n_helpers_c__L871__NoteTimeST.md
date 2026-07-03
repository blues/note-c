# `NoteTimeST`

## Status

Finalized. Source-inspected public session-time retrieval and refresh helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:871`
- Declaration/interface: `note.h:1714`
- Signature/API shape: `JTIME NoteTimeST(void)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NoteTimeST`
- Macro aliases/wrappers: none
- Ownership/lifetime role: maintains global cached time/zone state; owns no heap memory

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:243:_GetMs` finalized
  - `function:n_helpers.c:2301:_timerExpiredSecs` finalized
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `macro:note.h:479:NoteResponseError` finalized
  - `function:n_cjson_helpers.c:122:JGetInt` finalized
  - `function:n_helpers.c:791:_setTime` finalized in this slice
  - `function:n_cjson_helpers.c:27:JGetString` finalized
  - `function:n_str.c:31:strlcpy` finalized
  - `macro:note.h:486:NoteDeleteResponse` finalized
- External/system dependencies: `strchr`, `memcmp`
- Data/type/global dependencies: reads/writes `timeBaseSec`, `timeBaseSetAtMs`, `timeBaseSetManually`, `timeTimer`, `timeRefreshTimer`, `refreshTimerSecs`, `suppressionTimerSecs`, `zoneStillUnavailable`, `zoneForceRefresh`, `curZone`, `curZoneOffsetMins`, `curCountry`, and `curArea`
- Artifact coverage: generated artifact exists

## Behavior

Reads the current millisecond tick, adjusts `timeBaseSec` across 32-bit millisecond rollover, and expires the refresh timer by clearing `timeTimer` when configured. If time was not manually set and the suppression timer permits a refresh, it requests `card.time`, ignores null/error responses, and on a nonzero `"time"` field calls `_setTime(seconds)`.

When a nonempty `"zone"` field is present, it copies the substring before the first comma as the zone abbreviation, sets `zoneStillUnavailable` based on whether that abbreviation is `UTC`, clears `zoneForceRefresh`, stores the zone, offset minutes, country, and area, then deletes the response. The return value is the cached base seconds plus elapsed whole seconds since `timeBaseSetAtMs`; when no real base exists this acts like seconds since boot.

## Important Boundaries

- Preconditions and assumptions: global time state is initialized at module scope; request helpers own/delete request objects.
- Error/null/empty behavior: null/error `card.time` responses leave cached state unchanged and return elapsed boot seconds if no base is known; missing/zero `"time"` does not call `_setTime`; missing/empty `"zone"` does not update region globals.
- Ownership and lifetime: borrows strings from the response only until copied into global buffers; deletes any non-null response after inspection.
- Concurrency/global state: reads and mutates global time/zone state without locking.
- Portability/platform constraints: depends on platform millisecond hook width and wrap behavior through `_GetMs`.
- Performance shape: O(1) normally, plus one request transaction and bounded string copies when refresh is attempted.
- Security or parsing constraints: zone parsing expects comma-delimited `"ABBR,Name"` text; strings are bounded with `strlcpy`.

## Direct Call Sites

Exact source search found direct production callers in this file:

- `NoteTimeValidST`, `NoteTime`, `NoteRegion`, and `NoteLocalTimeST`.

## Role In Callers

This is the central cached time/zone refresh API. `NoteTime` and `NoteTimeValidST` are small public wrappers; region/local-time helpers use it to keep cached zone state fresh.

## Tests Observed

Focused `test/src/NoteTime_test.cpp` covers null response, error/missing time response, valid `card.time` response with zone fields, and millisecond rollover through `NoteTime`.

## Graph Links

- Graph node id: `function:n_helpers.c:871:NoteTimeST`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_GetMs`, `_timerExpiredSecs`, `NoteNewRequest`, `NoteRequestResponse`, `NoteResponseError`, `JGetInt`, `_setTime`, `JGetString`, `strlcpy`, and `NoteDeleteResponse`.

## Evidence Gaps

No unresolved source gaps for this node. Region/local-time callers remain separate frontier work.
