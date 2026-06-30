# `CARD_REQUEST_RETRIES_ALLOWED`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:98`
- Declaration/interface: request retry macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `5` retries.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: request transaction path.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Sets the retry allowance for `_noteTransactionShouldLock`; the loop condition is `<=`, so it permits the initial attempt plus five retry iterations.

## Important Boundaries

- Compile-time constant.
- Heartbeat responses decrement the loop counter and do not consume this retry budget.
- Fatal request errors break without exhausting retries.

## Direct Call Sites

Used by `_noteTransactionShouldLock` as the retry loop upper bound.

## Role In Callers

Bounds automatic retries for I/O errors, missing responses, corrupt responses, and CRC errors.

## Tests Observed

Focused transaction retry tests exercise retry and heartbeat handling.

## Graph Links

- Graph node id: `macro:n_lib.h:98:CARD_REQUEST_RETRIES_ALLOWED`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `_noteTransactionShouldLock`.

## Evidence Gaps

No unresolved source gaps for this node.
