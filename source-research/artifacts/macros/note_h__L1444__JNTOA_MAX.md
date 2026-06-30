# `JNTOA_MAX`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:1444`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `JNTOA_MAX`
- Macro aliases/wrappers: literal `(44)`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, field access, or wrapper expression.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the required buffer size for number-to-string conversion, including the null terminator.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers can use this value/expression wherever `note.h` is included.
- Exact source search distinguishes macro use from related literal or function use.

## Direct Call Sites

Used by `JNtoA`, `fmtflt`, cJSON `_print_number`, and environment number helpers in `n_helpers.c`.

## Role In Callers

Bounds stack buffers used for numeric formatting across ftoa, JSON printing, and environment helpers.

## Tests Observed

Focused `JNtoA_test.cpp` uses `char numStr[JNTOA_MAX]`; cJSON print tests exercise number output.

## Graph Links

- Graph node id: `macro:note.h:1444:JNTOA_MAX`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to number-format and helper call sites.

## Evidence Gaps

No unresolved source gaps for this node.
