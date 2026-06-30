# `NoteErrorContains`

## Status

Finalized. Source-inspected raw error substring helper.

## Identity

- Kind: function
- Definition: `n_request.c:940`
- Declaration/interface: public note-c request API declaration in headers
- Signature/API shape: `bool NoteErrorContains(const char *errstr, const char *errtype)`
- Visibility: public note-c API
- Public/exported names: `NoteErrorContains`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `strstr`
- Data/type/global dependencies: none

## Behavior

Returns whether `strstr(errstr, errtype)` is non-null.

## Important Boundaries

- Preconditions and assumptions: both input pointers must be non-null.
- Error/null/empty behavior: no null guards and no structured error parsing.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: substring search cost.
- Portability/platform constraints: bytewise case-sensitive C string search.

## Direct Call Sites

Exact source search found callers in `_noteTransactionShouldLock` for transaction `{io}` retry detection and in binary helper paths for bad-bin handling.

## Role In Callers

The transaction path uses this helper to decide whether transport errors are retryable I/O errors.

## Tests Observed

Coverage is indirect through `NoteTransaction_test.cpp` retry cases and binary bad-bin tests.

## Graph Links

- Graph node id: `function:n_request.c:940:NoteErrorContains`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No dedicated direct test was identified for this helper.
