# `NoteSetSTSecs`

## Status

Finalized. Source-inspected public suppression-timer interval setter.

## Identity

- Kind: function
- Definition: `n_helpers.c:948`
- Declaration/interface: `note.h:1690`
- Public/exported names: `NoteSetSTSecs`
- Macro aliases/wrappers: none found
- Signature/API shape: `uint32_t NoteSetSTSecs(uint32_t secs)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: updates module-local suppression timer configuration and returns the previous value

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads and writes static `suppressionTimerSecs`, which defaults to 10 seconds and is consumed by multiple `*ST` cached-helper paths
- Artifact coverage: source-inspected artifact

## Behavior

Copies the current `suppressionTimerSecs` value into a local `prev`, assigns the caller-provided `secs` to `suppressionTimerSecs`, and returns `prev`.

## Important Boundaries

- Preconditions and assumptions: accepts a primitive `uint32_t` second count; no pointer or ownership boundary.
- Error/null/empty behavior: no error path. Passing zero stores zero, making downstream `_timerExpiredSecs(..., suppressionTimerSecs)` calls use a zero-second interval.
- Ownership and lifetime: writes only module-local static state.
- Concurrency/global state: no locking around `suppressionTimerSecs`; cached-helper readers can observe unsynchronized updates.
- Portability/platform constraints: downstream timer expiration multiplies seconds by 1000, so large values can overflow there.
- Performance shape: constant-time read, write, and return.
- Security or parsing constraints: no range validation.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1690` and the definition in `n_helpers.c:948`. No production repository caller, test caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public configuration helper for downstream callers that want to change the suppression timer used by cached `*ST` helpers.

## Tests Observed

No focused tests for `NoteSetSTSecs` were found. Gaps include previous-value return behavior, zero-second behavior, large-value overflow effects in `_timerExpiredSecs`, and behavior across `NoteTimeST`, `NoteLocationValidST`, `NoteIsConnectedST`, and `NoteGetStatusST`.

## Graph Links

- Graph node id: `function:n_helpers.c:948:NoteSetSTSecs`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
