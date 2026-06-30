# `NoteGetMs`

## Status

Finalized. Source-inspected platform millisecond hook dispatcher.

## Identity

- Kind: function
- Definition: `n_hooks.c:520`
- Declaration/interface: `note.h:1063`
- Signature/API shape: `uint32_t NoteGetMs(void)`
- Visibility: public hook API
- Public/exported names: `NoteGetMs`
- Macro aliases/wrappers: `_GetMs` in `n_lib.h:243`
- Ownership/lifetime role: no ownership; returns a platform-provided counter value

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `hookGetMs`

## Behavior

Reads the global `hookGetMs` platform hook. If the hook is unset, returns `0`. If the hook is set, returns `hookGetMs()` unchanged.

`hookGetMs` is installed through `NoteSetFnDefault` only when no hook is already present, or overwritten through `NoteSetFn`.

## Important Boundaries

- Preconditions and assumptions: the installed hook must provide a millisecond-scale `uint32_t` value with wraparound behavior acceptable to callers.
- Error/null/empty behavior: unset hook returns `0`; there is no error signal.
- Ownership and lifetime: no allocation or pointer ownership.
- Concurrency/global state: reads global hook pointer; setters update it under the Notecard lock.
- Performance shape: O(1) plus hook cost.
- Portability/platform constraints: note-c does not provide a system clock fallback; platform timing remains behind the hook.

## Direct Call Sites

Exact source search found public declaration and many internal calls through the `_GetMs` macro alias. Direct textual `NoteGetMs` use appears in tests/fakes and in the `_GetMs` macro. Production timing consumers use `_GetMs` in request retry, ping input drain, serial/I2C transport loops, time helpers, `_timerExpiredSecs`, and debug-sync polling.

## Role In Callers

Provides the timing substrate for elapsed-time checks, retry deadlines, serial/I2C receive loops, reset drain loops, time-base maintenance, and suppression timers. Because unset hook returns zero, elapsed-time callers depend on platform initialization for meaningful timeout behavior.

## Tests Observed

`test/src/NoteGetMs_test.cpp` covers unset hook returning `0` and set hook returning the hook value. Transport, retry, time, and debug-sync tests fake `NoteGetMs` for elapsed-time behavior.

## Graph Links

- Graph node id: `function:n_hooks.c:520:NoteGetMs`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
