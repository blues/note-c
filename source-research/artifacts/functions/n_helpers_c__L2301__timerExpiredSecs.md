# `_timerExpiredSecs`

## Status

Finalized. Source-inspected suppression timer helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:2301`
- Declaration/interface: `n_helpers.c:78`; exposed to tests through `test/include/test_static.h:29` when static helpers are test-visible
- Signature/API shape: `NOTE_C_STATIC bool _timerExpiredSecs(uint32_t *timer, uint32_t periodSecs)`
- Visibility: internal helper
- Public/exported names: none
- Macro aliases/wrappers: none
- Ownership/lifetime role: mutates caller-provided timer state in place

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:243:_GetMs` finalized
- External/system dependencies: none
- Data/type/global dependencies: caller-owned `uint32_t *timer`; platform millisecond hook through `_GetMs`

## Behavior

Reads the previous timer value, gets the current millisecond counter through `_GetMs`, and treats the timer as expired when any of these conditions hold:

- the current counter is less than the previous timer value, interpreted as clock reset/wrap/backward movement;
- the previous timer value is `0`, interpreted as uninitialized;
- the current counter is greater than or equal to `prev + periodSecs * 1000`.

When expired, it writes the current millisecond counter back to `*timer` and returns `true`. Otherwise it leaves `*timer` unchanged and returns `false`.

## Important Boundaries

- Preconditions and assumptions: caller passes a non-null timer pointer; source has no null guard.
- Error/null/empty behavior: no error reporting. A zero previous timer expires immediately.
- Ownership and lifetime: no allocation; caller owns timer storage.
- Concurrency/global state: reads platform time through `_GetMs`; mutates shared/static timer globals when callers pass them.
- Performance shape: O(1).
- Portability/platform constraints: depends on the platform `NoteGetMs` hook. If no hook is installed and `_GetMs` returns `0`, an uninitialized timer remains zero and repeated calls continue to expire.
- Arithmetic constraints: interval calculation uses 32-bit unsigned arithmetic in `prev + (periodSecs * 1000)`. The source explicitly handles `now < prev`, but does not otherwise special-case multiplication or addition wraparound.

## Direct Call Sites

Exact source search found internal callers in `n_helpers.c`:

- `NoteTimeST` uses it for refresh and suppression timers before requesting `card.time`.
- `NoteLocationValidST` uses it to suppress repeated `card.location` polling.
- `NoteIsConnectedST` uses it to suppress repeated `hub.status` polling.
- `NoteGetServiceConfigST` uses it with a four-hour period to refresh cached `hub.get` service configuration.
- `NoteGetStatusST` uses it to suppress repeated `card.status` polling.

## Role In Callers

Provides shared cache/suppression timing for high-level helper APIs so repeated calls can reuse cached state until the configured interval elapses. Reset-style public wrappers zero their associated timer globals before calling the suppressed variants when they need an immediate refresh.

## Tests Observed

No dedicated direct `_timerExpiredSecs` test was found. Focused helper tests for time, location, connectivity, service config, status, and debug-sync paths fake `NoteGetMs` or exercise timer-gated callers indirectly.

## Graph Links

- Graph node id: `function:n_helpers.c:2301:_timerExpiredSecs`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
