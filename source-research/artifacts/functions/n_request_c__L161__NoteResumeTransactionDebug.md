# `NoteResumeTransactionDebug`

## Status

Final. Behavior, public declaration, wrapper role, direct call sites, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_request.c:161`
- Declaration/interface: `note.h:409`
- Public/exported names: `NoteResumeTransactionDebug` (`NOTE_C_DEPRECATED`)
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteResumeTransactionDebug(void)`
- Visibility: deprecated public SDK API
- Ownership/lifetime role: no ownership; public wrapper around transaction-debug suppression counter

## Dependency Status

- Project-local dependencies:
- `function:n_request.c:80:_noteResumeTransactionDebug` (calls, verified)
- External/system dependencies: none
- Data/type/global dependencies: indirectly decrements file-local `suppressShowTransactions`
- Artifact coverage: generated artifact exists

## Behavior

Deprecated public wrapper that calls `_noteResumeTransactionDebug()` exactly once. It resumes transaction/error trace output by decrementing the shared suppression counter.

## Important Boundaries

- Preconditions and assumptions: intended to balance a previous `NoteSuspendTransactionDebug()` call
- Error/null/empty behavior: no arguments, no return value, and no validation
- Ownership and lifetime: no allocations or ownership transfer
- Concurrency/global state: mutates shared suppression state through the internal helper without taking a lock
- Portability/platform constraints: none
- Performance shape: constant-time wrapper call
- Security or parsing constraints: no parsing; affects diagnostic output visibility only

## Direct Call Sites

No production library callers found outside the definition and public declaration. Focused/unit-test callers:

- `test/src/NoteResumeTransactionDebug_test.cpp:31`
- `test/src/NoteTransaction_test.cpp:241`

## Role In Callers

Application-facing compatibility wrapper for resuming transaction debug output after public suppression.

## Tests Observed

`test/src/NoteResumeTransactionDebug_test.cpp` fakes `_noteResumeTransactionDebug()` and verifies this public API calls it once.

## Graph Links

- Graph node id: `function:n_request.c:161:NoteResumeTransactionDebug`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No direct test covers unbalanced public suspend/resume nesting; behavior follows the finalized internal helper.
