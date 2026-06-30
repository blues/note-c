# `_GetMs`

## Status

Finalized. Source-inspected millisecond hook macro alias.

## Identity

- Kind: macro
- Definition: `n_lib.h:243`
- Declaration/interface: internal readability wrapper
- Signature/API shape: `_GetMs`
- Visibility: internal macro
- Public/exported names: none
- Macro aliases/wrappers: expands to `NoteGetMs`

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:520:NoteGetMs` finalized
- External/system dependencies: none
- Data/type/global dependencies: `hookGetMs` through `NoteGetMs`
- Graph repair: verified existing `_GetMs` -> `NoteGetMs` macro-alias edge from source

## Behavior

Expands directly to `NoteGetMs`. All null-hook and dispatch behavior is inherited from the finalized target: unset timing hook returns `0`; set timing hook is called and its return value is forwarded.

## Important Boundaries

- Preconditions and assumptions: function-like calls through the alias rely on `NoteGetMs` hook semantics.
- Error/null/empty behavior: unset hook returns `0` through the target.
- Ownership and lifetime: no allocation.
- Concurrency/global state: reads the timing hook via `NoteGetMs`.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: portable alias; platform timing remains behind the hook target.

## Direct Call Sites

Exact source search found production callers in request retry/ping paths (`n_request.c`), serial transport loops (`n_serial.c`), I2C transport loops (`n_i2c.c`), time helpers and suppression timers (`n_helpers.c`), and debug-sync polling (`n_helpers.c`).

## Role In Callers

Supplies elapsed-time values for timeout loops, retry deadlines, reset drains, time-base maintenance, suppression timers, and polling intervals. `_timerExpiredSecs` uses this alias to decide when cached helper state should be refreshed.

## Tests Observed

`NoteGetMs_test.cpp` covers target hook behavior. Focused caller tests fake `NoteGetMs` in retry, time, serial, I2C, and debug-sync paths.

## Graph Links

- Graph node id: `macro:n_lib.h:243:_GetMs`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
