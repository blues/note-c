# `FALSE`

## Status

Finalized. Source-inspected local boolean compatibility macro.

## Identity

- Kind: macro
- Definition: `n_atof.c:37`
- Declaration/interface: local translation-unit macro, not exported by a public header
- Public/exported names: none
- Macro aliases/wrappers: none
- Signature/API shape: object-like macro `FALSE 0`, defined with `TRUE` only when `TRUE` is not already defined
- Visibility: local to `n_atof.c`
- Ownership/lifetime role: compile-time integer constant for parser state flags

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: preprocessor symbol `TRUE`; this fallback `FALSE` is emitted only inside the `#ifndef TRUE` block
- Data/type/global dependencies: used by `JAtoN` sign and exponent-sign state variables
- Artifact coverage: source-inspected artifact

## Behavior

Defines `FALSE` as integer constant `0` when the local `TRUE`/`FALSE` fallback block is active. `JAtoN` uses it to initialize and clear mantissa and exponent sign flags.

## Important Boundaries

- Preconditions and assumptions: only defined when `TRUE` is absent; if an environment defines `TRUE` without `FALSE`, this fallback does not define `FALSE`.
- Error/null/empty behavior: no runtime behavior beyond integer constant substitution.
- Ownership and lifetime: preprocessor-only.
- Concurrency/global state: none.
- Portability/platform constraints: preserves external boolean macro definitions.
- Performance shape: compile-time substitution only.
- Security or parsing constraints: parser behavior assumes `FALSE` is zero.

## Direct Call Sites

Exact source inspection found `FALSE` uses only inside `n_atof.c` in `JAtoN` sign/exponent parsing.

## Role In Callers

Local compatibility constant for the `JAtoN` parser.

## Tests Observed

No focused tests target this macro directly.

## Graph Links

- Graph node id: `macro:n_atof.c:37:FALSE`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none.

## Evidence Gaps

No unresolved source gaps for this node.
