# `TRUE`

## Status

Finalized. Source-inspected local boolean compatibility macro.

## Identity

- Kind: macro
- Definition: `n_atof.c:36`
- Declaration/interface: local translation-unit macro, not exported by a public header
- Public/exported names: none
- Macro aliases/wrappers: none
- Signature/API shape: object-like macro `TRUE 1`, defined only when `TRUE` is not already defined
- Visibility: local to `n_atof.c`
- Ownership/lifetime role: compile-time integer constant for parser state flags

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: preprocessor symbol `TRUE` if provided externally
- Data/type/global dependencies: used by `JAtoN` sign and exponent-sign state variables
- Artifact coverage: source-inspected artifact

## Behavior

Defines `TRUE` as integer constant `1` if no prior `TRUE` macro exists. `JAtoN` assigns it to `sign` and `expSign` when a negative mantissa or exponent is parsed, and when a computed exponent is negative.

## Important Boundaries

- Preconditions and assumptions: only used if the environment has not already defined `TRUE`.
- Error/null/empty behavior: no runtime behavior beyond integer constant substitution.
- Ownership and lifetime: preprocessor-only.
- Concurrency/global state: none.
- Portability/platform constraints: preserves external `TRUE` definitions.
- Performance shape: compile-time substitution only.
- Security or parsing constraints: parser behavior assumes `TRUE` is truthy.

## Direct Call Sites

Exact source inspection found `TRUE` uses only inside `n_atof.c` in `JAtoN` sign/exponent parsing.

## Role In Callers

Local compatibility constant for the `JAtoN` parser.

## Tests Observed

No focused tests target this macro directly.

## Graph Links

- Graph node id: `macro:n_atof.c:36:TRUE`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none.

## Evidence Gaps

No unresolved source gaps for this node.
