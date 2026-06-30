# `_noteSerialReceive`

## Status

Final. Behavior, internal declaration, hook dispatch, default behavior, direct call sites, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:902`
- Declaration/interface: `n_lib.h:144`
- Public/exported names: internal `_noteSerialReceive`; public hook setters are `NoteSetFnSerial` and `NoteSetFnSerialDefault`
- Macro aliases/wrappers: `_SerialReceive` expands to `_noteSerialReceive` at `n_lib.h:233`
- Signature/API shape: `char _noteSerialReceive(void)`
- Visibility: internal serial receive dispatch helper
- Ownership/lifetime role: returns one character from platform hook; no ownership

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: platform serial receive hook
- Data/type/global dependencies: reads `hookActiveInterface` and `hookSerialReceive`
- Artifact coverage: generated artifact exists

## Behavior

Returns `hookSerialReceive()` only when the active interface is serial and the receive hook is set. Otherwise returns `'\0'`.

## Important Boundaries

- Preconditions and assumptions: callers generally check `_SerialAvailable()` before receiving
- Error/null/empty behavior: inactive interface or missing hook returns NUL character
- Ownership and lifetime: no allocation or ownership
- Concurrency/global state: reads global active-interface and hook state without local locking
- Portability/platform constraints: platform receive behavior stays behind hook
- Performance shape: constant-time gate plus platform hook call
- Security or parsing constraints: no parsing

## Direct Call Sites

Direct internal macro callers use `_SerialReceive` in `n_serial.c:190`, `n_serial.c:263`, and `n_request.c:803`. Focused direct tests are in `test/src/_noteSerialReceive_test.cpp`.

## Role In Callers

Serial one-byte receive primitive for reset drain, chunked receive, and ping-drain paths.

## Tests Observed

`test/src/_noteSerialReceive_test.cpp` verifies inactive interface or missing hook returns `'\0'`, and active serial plus a hook returns the hook result.

## Graph Links

- Graph node id: `function:n_hooks.c:902:_noteSerialReceive`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
