# `CARD_INTRA_TRANSACTION_TIMEOUT_SEC`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:45`
- Declaration/interface: shared internal timeout macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `1` second.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: I/O and helper translation units including `n_lib.h`.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Sets the short intra-transaction receive timeout after a transaction has begun or after the first bytes arrive.

## Important Boundaries

- Compile-time constant.
- Callers multiply by 1000 for millisecond hook APIs.
- Distinct from the longer inter-transaction request timeout.

## Direct Call Sites

Used by `NoteBinaryStoreReceive`, `_i2cNoteTransaction`, `_i2cChunkedReceive`, `_serialNoteTransaction`, and `_serialChunkedReceive`.

## Role In Callers

Bounds packet completion waits once a receive operation is in progress.

## Tests Observed

Focused I/O and helper tests exercise receive timeout paths through these functions.

## Graph Links

- Graph node id: `macro:n_lib.h:45:CARD_INTRA_TRANSACTION_TIMEOUT_SEC`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges from binary, I2C, and serial receive call sites.

## Evidence Gaps

No unresolved source gaps for this node.
