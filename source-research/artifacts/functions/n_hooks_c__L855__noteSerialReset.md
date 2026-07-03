# `_noteSerialReset`

## Status

Finalized. Source-inspected serial reset hook wrapper, hook/global boundaries, macro alias caller, and focused tests.

## Identity

- Kind: function
- Definition: `n_hooks.c:855`
- Declaration/interface: `n_lib.h:141`
- Public/exported names: none; internal hook wrapper
- Macro aliases/wrappers: `_SerialReset` expands to `_noteSerialReset` at `n_lib.h:230`
- Signature/API shape: `bool _noteSerialReset(void)`
- Visibility: internal declaration in `n_lib.h`, implementation in `n_hooks.c`
- Ownership/lifetime role: no allocation or ownership transfer; dispatches an optional platform reset hook

## Dependency Status

- Project-local dependencies:
  - No project-local callees
- External/system dependencies: optional caller-supplied `serialResetFn` hook
- Data/type/global dependencies: reads `hookActiveInterface` and `hookSerialReset`
- Artifact coverage: source, declaration, macro alias, hook state, direct caller search, and focused tests inspected

## Behavior

If `hookActiveInterface == NOTE_C_INTERFACE_SERIAL` and `hookSerialReset` is non-null, calls `hookSerialReset()` and returns its bool result. Otherwise returns true without calling a platform reset hook.

## Important Boundaries

- Preconditions and assumptions: serial reset hook installation and active-interface selection are managed by the hook configuration APIs.
- Error/null/empty behavior: inactive serial interface or null reset hook is treated as successful reset.
- Ownership and lifetime: no allocation and no ownership transfer.
- Concurrency/global state: reads global active-interface and hook-pointer state; caller/higher-level setters provide synchronization.
- Portability/platform constraints: platform-specific reset behavior remains behind `serialResetFn`.
- Performance shape: O(1) plus optional hook cost.
- Security or parsing constraints: none.

## Direct Call Sites

Exact source search found:

- `n_lib.h:141` internal declaration.
- `n_lib.h:230` `_SerialReset` macro alias.
- `n_hooks.c:855` implementation.

Focused tests fake `_noteSerialReset` in `_serialNoteReset_test.cpp`; hook setup/getter tests inspect the underlying `hookSerialReset` global.

## Role In Callers

`_SerialReset` is the project-internal readability alias used by the serial bus reset/resynchronization implementation. When reached through that alias, inactive or unconfigured serial reset hooks are considered success by this wrapper.

## Tests Observed

`test/src/_noteSerialReset_test.cpp` directly covers inactive interface with null/non-null reset hook returning true, active serial with null reset hook returning true, and active serial with non-null reset hook returning the hook value. `_serialNoteReset_test.cpp` fakes this wrapper to cover the higher-level serial reset caller's failure and retry behavior.

## Test Coverage Notes

Focused tests do not exercise concurrent hook mutation during a reset call or external platform reset implementations.

## Graph Links

- Graph node id: `function:n_hooks.c:855:_noteSerialReset`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
