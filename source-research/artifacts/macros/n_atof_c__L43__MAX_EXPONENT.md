# `MAX_EXPONENT`

## Status

Finalized. Source-inspected local decimal-exponent clamp for `JAtoN`.

## Identity

- Kind: macro
- Definition: `n_atof.c:43`
- Declaration/interface: local translation-unit macro, not exported by a public header
- Public/exported names: none
- Macro aliases/wrappers: none
- Signature/API shape: object-like macro `MAX_EXPONENT 511`
- Visibility: local to `n_atof.c`
- Ownership/lifetime role: compile-time numeric bound for floating-point exponent processing

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: used by `JAtoN` to cap absolute decimal exponent magnitude
- Artifact coverage: source-inspected artifact

## Behavior

Defines the largest decimal exponent the parser will process as `511`. After the parsed exponent and fractional adjustment are combined and made positive, `JAtoN` clamps values larger than `MAX_EXPONENT` down to this bound before multiplying or dividing by powers of ten.

## Important Boundaries

- Preconditions and assumptions: constant is tuned to the legacy parser's overflow/underflow avoidance strategy.
- Error/null/empty behavior: no runtime behavior beyond constant substitution.
- Ownership and lifetime: preprocessor-only.
- Concurrency/global state: none.
- Portability/platform constraints: affects floating-point conversion behavior consistently within this translation unit.
- Performance shape: one integer comparison and assignment at the use site.
- Security or parsing constraints: very large exponents are deliberately saturated for conversion work rather than tracked exactly.

## Direct Call Sites

Exact source inspection found two `JAtoN` uses in `n_atof.c`: a comparison `exp > MAX_EXPONENT` and assignment `exp = MAX_EXPONENT`.

## Role In Callers

Local guard for `JAtoN` exponent processing.

## Tests Observed

No focused tests target this macro directly.

## Graph Links

- Graph node id: `macro:n_atof.c:43:MAX_EXPONENT`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none.

## Evidence Gaps

No unresolved source gaps for this node.
