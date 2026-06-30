# `_noteSerialAvailable`

## Status

Final. Behavior, internal declaration, hook dispatch, default behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:887`
- Declaration/interface: `n_lib.h:143`
- Public/exported names: internal `_noteSerialAvailable`; public hook setters are `NoteSetFnSerial` and `NoteSetFnSerialDefault`
- Macro aliases/wrappers: `_SerialAvailable` expands to `_noteSerialAvailable` at `n_lib.h:232`
- Signature/API shape: `bool _noteSerialAvailable(void)`
- Visibility: internal serial availability dispatch helper
- Ownership/lifetime role: queries platform hook; no ownership

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: platform serial available hook
- Data/type/global dependencies: reads `hookActiveInterface` and `hookSerialAvailable`
- Artifact coverage: generated artifact exists

## Behavior

Returns `hookSerialAvailable()` only when the active interface is serial and the availability hook is set. Otherwise returns `false`.

## Important Boundaries

- Preconditions and assumptions: serial interface and availability hook must be configured for true availability reports
- Error/null/empty behavior: inactive interface or missing hook returns `false`
- Ownership and lifetime: no allocation or ownership
- Concurrency/global state: reads global active-interface and hook state without local locking
- Portability/platform constraints: platform availability behavior stays behind hook
- Performance shape: constant-time gate plus platform hook call
- Security or parsing constraints: no parsing

## Direct Call Sites

Direct internal macro callers use `_SerialAvailable` in `n_serial.c:70`, `n_serial.c:185`, `n_serial.c:243`, and `n_request.c:802`. Focused direct tests are in `test/src/_noteSerialAvailable_test.cpp`.

## Role In Callers

Serial availability primitive for response wait loops, reset drain, chunked receive, and ping-drain.

## Tests Observed

`test/src/_noteSerialAvailable_test.cpp` verifies inactive interface or missing hook returns `false`, and active serial plus a hook returns the hook result.

## Graph Links

- Graph node id: `function:n_hooks.c:887:_noteSerialAvailable`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
