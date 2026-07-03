# `const`

## Status

Finalized. Source-inspected legacy C compatibility macro local to `n_atof.c`.

## Identity

- Kind: macro
- Definition: `n_atof.c:29`
- Declaration/interface: local translation-unit macro, not exported by a public header
- Public/exported names: none
- Macro aliases/wrappers: maps `const` to `__const__` for GNU non-`__STDC__` builds, or to an empty token for other non-`__STDC__` builds
- Signature/API shape: object-like compatibility macro under `#ifndef __STDC__`
- Visibility: local to `n_atof.c` after its definition
- Ownership/lifetime role: preprocessor-only type-qualifier compatibility shim

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: compiler preprocessor symbols `__STDC__` and `__GNUC__`
- Data/type/global dependencies: affects later `const` tokens in `JAtoN` declarations when compiling as non-standard C
- Artifact coverage: source-inspected artifact

## Behavior

In standard C builds this macro is not defined. In non-`__STDC__` GNU builds it rewrites `const` to `__const__`; in other non-standard builds it removes the `const` qualifier. This lets the legacy locale-independent `JAtoN` implementation compile on older compilers.

## Important Boundaries

- Preconditions and assumptions: only intended for old non-standard C compiler modes.
- Error/null/empty behavior: no runtime behavior.
- Ownership and lifetime: preprocessor-only; no storage or ownership.
- Concurrency/global state: none.
- Portability/platform constraints: deliberately compiler-variant dependent.
- Performance shape: compile-time token substitution only.
- Security or parsing constraints: removing `const` in old compiler modes weakens compile-time immutability checks.

## Direct Call Sites

Source inspection shows later `const` uses in the `JAtoN` signature and local pointers in `n_atof.c`. No public header exposure or repository-wide macro alias was found.

## Role In Callers

Local compatibility support for the `JAtoN` ASCII-to-number parser implementation.

## Tests Observed

No focused tests target this compiler-compatibility macro directly.

## Graph Links

- Graph node id: `macro:n_atof.c:29:const`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none.

## Evidence Gaps

No unresolved source gaps for this node.
