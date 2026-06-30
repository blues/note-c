# `_noteResumeTransactionDebug`

## Status

Final. Behavior, declaration, direct call sites, global state effect, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_request.c:80`
- Declaration/interface: `n_lib.h:121`
- Public/exported names: internal helper only; public wrapper is `NoteResumeTransactionDebug`
- Macro aliases/wrappers: none found
- Signature/API shape: `void _noteResumeTransactionDebug(void)`
- Visibility: internal SDK helper declared in `n_lib.h`
- Ownership/lifetime role: adjusts file-local transaction-debug suppression state; no object ownership

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none
- Data/type/global dependencies: decrements file-local `suppressShowTransactions` in `n_request.c`
- Artifact coverage: generated artifact exists

## Behavior

Resumes transaction/error trace output by decrementing `suppressShowTransactions`. `_errDoc()` prints error details only when that counter is exactly `0`; this helper is the low-level operation used to undo `_noteSuspendTransactionDebug()`.

## Important Boundaries

- Preconditions and assumptions: callers should balance this with a prior `_noteSuspendTransactionDebug()` call
- Error/null/empty behavior: no arguments, no return value, and no underflow/bounds check; extra resumes can make the counter negative
- Ownership and lifetime: no allocations or ownership transfer
- Concurrency/global state: mutates `suppressShowTransactions` without locking; callers rely on surrounding transaction sequencing where needed
- Portability/platform constraints: none beyond global state shared across the SDK instance
- Performance shape: constant-time integer decrement
- Security or parsing constraints: no parsing; affects diagnostic output visibility only

## Direct Call Sites

Verified direct callers:

- `n_request.c:163` in `NoteResumeTransactionDebug`
- `n_request.c:889` in the early-failure path of `NotePing`
- `n_request.c:910` in the normal `NotePing` cleanup path
- `test/src/_errDoc_test.cpp:69` after suppressing `_errDoc` logging

## Role In Callers

Balances a prior suspend operation so public calls and internal ping/autobaud probes can restore normal transaction/error trace behavior.

## Tests Observed

Focused evidence:

- `test/src/NoteResumeTransactionDebug_test.cpp` verifies the public wrapper calls `_noteResumeTransactionDebug()` exactly once.
- `test/src/_errDoc_test.cpp` uses `_noteSuspendTransactionDebug()` and `_noteResumeTransactionDebug()` around `_errDoc()` to verify suppressed error logging is restored after the scoped suppression.

## Graph Links

- Graph node id: `function:n_request.c:80:_noteResumeTransactionDebug`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

The helper itself has no direct unit test for negative counter behavior; this is source-observed behavior from the unchecked decrement.
