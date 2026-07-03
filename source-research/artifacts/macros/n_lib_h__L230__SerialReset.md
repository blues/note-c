# `_SerialReset`

## Status

Finalized. Source-inspected serial reset alias, finalized expansion target, direct caller sites, and focused target/caller tests.

## Identity

- Kind: macro
- Definition: `n_lib.h:230`
- Declaration/interface: internal readability wrapper in `n_lib.h`
- Public/exported names: none
- Macro aliases/wrappers: expands directly to `_noteSerialReset`
- Signature/API shape: `_SerialReset`
- Visibility: project-internal macro for translation units including `n_lib.h`
- Ownership/lifetime role: no allocation or ownership transfer; aliases a serial reset hook wrapper

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:855:_noteSerialReset` finalized
- External/system dependencies: optional platform serial reset hook through the expansion target
- Data/type/global dependencies: inherited from `_noteSerialReset`: active-interface state and `hookSerialReset`
- Artifact coverage: macro definition, expansion target, direct callers, and focused tests inspected

## Behavior

Expands directly to `_noteSerialReset`. Behavior is inherited from the finalized target: when serial is the active interface and a serial reset hook is registered, it returns that hook's bool result; otherwise it returns true.

## Important Boundaries

- Preconditions and assumptions: active interface and serial reset hook configuration are managed by hook setup APIs.
- Error/null/empty behavior: inactive interface or missing serial reset hook maps to success through the target wrapper.
- Ownership and lifetime: no allocation and no ownership transfer.
- Concurrency/global state: follows target global hook state; callers handle higher-level synchronization.
- Portability/platform constraints: adapter-specific reset behavior remains behind the platform hook.
- Performance shape: preprocessor alias plus O(1) target dispatch.
- Security or parsing constraints: none.

## Direct Call Sites

Exact production search found:

- `n_lib.h:230` macro definition.
- `n_serial.c:159` `_serialNoteReset` exits immediately if the initial `_SerialReset()` fails.
- `n_serial.c:209` `_serialNoteReset` retries `_SerialReset()` after failed drain/resync attempts and exits if the retry reset fails.

## Role In Callers

`_serialNoteReset` uses the alias as the low-level bus reset step before and during serial resynchronization. A false target result aborts the higher-level serial reset path.

## Tests Observed

`test/src/_noteSerialReset_test.cpp` directly covers the expansion target's active-interface and null-hook behavior. `test/src/_serialNoteReset_test.cpp` fakes `_noteSerialReset` to verify higher-level caller failure, retry, and success paths through the alias target.

## Test Coverage Notes

Focused tests do not exercise the preprocessor alias spelling separately from its target expansion.

## Graph Links

- Graph node id: `macro:n_lib.h:230:_SerialReset`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
