# `_noteSuspendTransactionDebug`

## Status

Final. Behavior, declaration, direct call sites, global state effect, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_request.c:88`
- Declaration/interface: `n_lib.h:122`
- Public/exported names: internal helper only; public wrapper is `NoteSuspendTransactionDebug`
- Macro aliases/wrappers: none found
- Signature/API shape: `void _noteSuspendTransactionDebug(void)`
- Visibility: internal SDK helper declared in `n_lib.h`
- Ownership/lifetime role: adjusts file-local transaction-debug suppression state; no object ownership

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none
- Data/type/global dependencies: increments file-local `suppressShowTransactions` in `n_request.c`
- Artifact coverage: generated artifact exists

## Behavior

Suppresses transaction/error trace output by incrementing `suppressShowTransactions`. `_errDoc()` skips error-detail debug output when that counter is nonzero.

## Important Boundaries

- Preconditions and assumptions: callers should later balance this with `_noteResumeTransactionDebug()`
- Error/null/empty behavior: no arguments, no return value, and no overflow/bounds check
- Ownership and lifetime: no allocations or ownership transfer
- Concurrency/global state: mutates `suppressShowTransactions` without locking; callers rely on surrounding transaction sequencing where needed
- Portability/platform constraints: none beyond global state shared across the SDK instance
- Performance shape: constant-time integer increment
- Security or parsing constraints: no parsing; affects diagnostic output visibility only

## Direct Call Sites

Verified direct callers:

- `n_request.c:155` in `NoteSuspendTransactionDebug`
- `n_request.c:885` in `NotePing` before a one-shot echo transaction
- `test/src/_errDoc_test.cpp:55` before checking suppressed `_errDoc` logging

## Role In Callers

Provides a nesting-capable suppression counter for public debug-suppression calls and for internal ping/autobaud probing where expected transport noise should not be logged as normal transaction output.

## Tests Observed

Focused evidence:

- `test/src/NoteSuspendTransactionDebug_test.cpp` verifies the public wrapper calls `_noteSuspendTransactionDebug()` exactly once.
- `test/src/_errDoc_test.cpp` verifies that `_errDoc()` does not emit error debug output while transactions are suppressed.

## Graph Links

- Graph node id: `function:n_request.c:88:_noteSuspendTransactionDebug`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

The helper itself has no direct unit test for overflow or unbalanced suppression behavior; this is source-observed behavior from the unchecked increment.
