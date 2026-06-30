# `NoteSuspendTransactionDebug`

## Status

Final. Behavior, public declaration, wrapper role, direct call sites, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_request.c:153`
- Declaration/interface: `note.h:408`
- Public/exported names: `NoteSuspendTransactionDebug` (`NOTE_C_DEPRECATED`)
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSuspendTransactionDebug(void)`
- Visibility: deprecated public SDK API
- Ownership/lifetime role: no ownership; public wrapper around transaction-debug suppression counter

## Dependency Status

- Project-local dependencies:
- `function:n_request.c:88:_noteSuspendTransactionDebug` (calls, verified)
- External/system dependencies: none
- Data/type/global dependencies: indirectly increments file-local `suppressShowTransactions`
- Artifact coverage: generated artifact exists

## Behavior

Deprecated public wrapper that calls `_noteSuspendTransactionDebug()` exactly once. It suppresses transaction/error trace output by incrementing the shared suppression counter.

## Important Boundaries

- Preconditions and assumptions: callers should later call `NoteResumeTransactionDebug()` to balance suppression
- Error/null/empty behavior: no arguments, no return value, and no validation
- Ownership and lifetime: no allocations or ownership transfer
- Concurrency/global state: mutates shared suppression state through the internal helper without taking a lock
- Portability/platform constraints: none
- Performance shape: constant-time wrapper call
- Security or parsing constraints: no parsing; affects diagnostic output visibility only

## Direct Call Sites

No production library callers found outside the definition and public declaration. Focused/unit-test callers:

- `test/src/NoteSuspendTransactionDebug_test.cpp:31`
- `test/src/NoteTransaction_test.cpp:237`

## Role In Callers

Application-facing compatibility wrapper for suppressing transaction debug output.

## Tests Observed

`test/src/NoteSuspendTransactionDebug_test.cpp` fakes `_noteSuspendTransactionDebug()` and verifies this public API calls it once.

## Graph Links

- Graph node id: `function:n_request.c:153:NoteSuspendTransactionDebug`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No direct test covers unbalanced public suspend/resume nesting; behavior follows the finalized internal helper.
