# `NoteSetRequestTimeout`

## Status

Finalized. Source-inspected public request-timeout override setter.

## Identity

- Kind: function
- Definition: `n_request.c:166`
- Declaration/interface: `note.h:470`
- Public/exported names: `NoteSetRequestTimeout`
- Macro aliases/wrappers: none found
- Signature/API shape: `uint32_t NoteSetRequestTimeout(uint32_t overrideSecs)`
- Visibility: public note-c request API implemented in `n_request.c`
- Ownership/lifetime role: updates process-global transaction timeout override state

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads effective `CARD_INTER_TRANSACTION_TIMEOUT_SEC`; writes `cardTransactionTimeoutOverrideSecs`
- Artifact coverage: source-inspected artifact

## Behavior

Computes the previous effective request timeout through `CARD_INTER_TRANSACTION_TIMEOUT_SEC`, stores `overrideSecs` in `cardTransactionTimeoutOverrideSecs`, and returns the previous effective value. Passing zero stores zero, which makes the macro fall back to the 30-second default.

## Important Boundaries

- Preconditions and assumptions: scalar input only.
- Error/null/empty behavior: no error return; zero is a valid value that restores default macro behavior.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: unsynchronized read/write of `cardTransactionTimeoutOverrideSecs`.
- Portability/platform constraints: keeps timeout policy in portable core state rather than platform adapters.
- Performance shape: one macro read, one scalar write, one return.
- Security or parsing constraints: no validation or range check; large values are stored as-is and downstream conversions own overflow implications.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:470`, the definition in `n_request.c:166`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public API for downstream code to override the effective request timeout used by transaction paths that reference `CARD_INTER_TRANSACTION_TIMEOUT_SEC`.

## Tests Observed

Focused tests in `test/src/NoteSetRequestTimeout_test.cpp` cover default-to-custom, custom-to-custom, restoring zero/default behavior, `UINT32_MAX`, and one-second values. Gaps: no concurrency coverage and no downstream timeout multiplication overflow assertion.

## Graph Links

- Graph node id: `function:n_request.c:166:NoteSetRequestTimeout`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
