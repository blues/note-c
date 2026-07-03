# `CARD_INTER_TRANSACTION_TIMEOUT_SEC`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:44`
- Declaration/interface: shared internal timeout macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: expands to `(cardTransactionTimeoutOverrideSecs == 0 ? 30 : cardTransactionTimeoutOverrideSecs)`.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: all translation units including `n_lib.h`.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Selects the default inter-transaction timeout in seconds. A zero override yields 30 seconds; `NoteSetRequestTimeout` writes the global override and returns the previous effective value.

## Important Boundaries

- Depends on global `cardTransactionTimeoutOverrideSecs` in `n_request.c`.
- Runtime mutable through `NoteSetRequestTimeout`.
- Converted to milliseconds by callers where needed.

## Direct Call Sites

Used by `_noteTransaction_calculateTimeoutMs`, `NoteSetRequestTimeout`, `NoteRequestResponseJSON`, and `_noteTransactionShouldLock` in `n_request.c`.

## Role In Callers

Establishes the lock/transaction timeout budget and the default request timeout before per-request `milliseconds` or `seconds` fields refine it.

## Tests Observed

Focused tests cover `NoteSetRequestTimeout` and `_noteTransaction_calculateTimeoutMs` behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:44:CARD_INTER_TRANSACTION_TIMEOUT_SEC`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges from the four `n_request.c` call sites.

## Evidence Gaps

No unresolved source gaps for this node.
