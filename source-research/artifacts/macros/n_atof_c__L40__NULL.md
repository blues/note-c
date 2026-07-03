# `NULL`

## Status

Finalized. Source-inspected local null-pointer compatibility macro.

## Identity

- Kind: macro
- Definition: `n_atof.c:40`
- Declaration/interface: local translation-unit macro, not exported by a public header
- Public/exported names: none
- Macro aliases/wrappers: none
- Signature/API shape: object-like macro `NULL 0`, defined only when `NULL` is not already defined
- Visibility: local to `n_atof.c`
- Ownership/lifetime role: compile-time null pointer constant for the parser implementation

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: preprocessor symbol `NULL` if provided by included headers
- Data/type/global dependencies: used by `JAtoN` to guard the optional `endPtr` output pointer
- Artifact coverage: source-inspected artifact

## Behavior

Defines `NULL` as `0` if no prior `NULL` macro exists. `JAtoN` compares `endPtr` against `NULL` before writing the parsed-end pointer.

## Important Boundaries

- Preconditions and assumptions: only intended as a fallback when included headers do not define `NULL`.
- Error/null/empty behavior: no runtime behavior beyond constant substitution.
- Ownership and lifetime: preprocessor-only.
- Concurrency/global state: none.
- Portability/platform constraints: preserves external `NULL` definitions.
- Performance shape: compile-time substitution only.
- Security or parsing constraints: integer `0` is used as the fallback null pointer constant.

## Direct Call Sites

Exact source inspection found the fallback definition and `JAtoN` optional-output checks in `n_atof.c`; no public header exposure was found.

## Role In Callers

Local compatibility constant for `JAtoN`'s optional `endPtr` output.

## Tests Observed

No focused tests target this macro directly.

## Graph Links

- Graph node id: `macro:n_atof.c:40:NULL`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none.

## Evidence Gaps

No unresolved source gaps for this node.
