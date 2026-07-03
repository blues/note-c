# `PRINT_TAB_CHARS`

## Status

Finalized. Source-inspected local formatted-JSON indentation macro.

## Identity

- Kind: macro
- Definition: `n_cjson.c:80`
- Declaration/interface: local translation-unit macro, not exported by a public header
- Public/exported names: none
- Macro aliases/wrappers: none
- Signature/API shape: object-like macro `PRINT_TAB_CHARS 4`
- Visibility: local to `n_cjson.c`
- Ownership/lifetime role: compile-time indentation width for formatted JSON printing

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: used by object/array print paths to size and emit indentation
- Artifact coverage: source-inspected artifact

## Behavior

Configures formatted JSON indentation to four space characters per nesting level. Print code has compile-time branches for `PRINT_TAB_CHARS == 0`, which would emit tab characters instead; the repository definition selects the space-emitting branch.

## Important Boundaries

- Preconditions and assumptions: local constant is used by buffer sizing and indentation loops.
- Error/null/empty behavior: no runtime behavior beyond constant substitution.
- Ownership and lifetime: preprocessor-only.
- Concurrency/global state: none.
- Portability/platform constraints: local formatting policy only.
- Performance shape: affects indentation loop count and buffer sizing during formatted printing.
- Security or parsing constraints: incorrect changes would risk print-buffer sizing mismatches in formatted output.

## Direct Call Sites

Source inspection found uses in formatted object/array printing branches in `n_cjson.c`, including indentation sizing, space/tab selection, and closing-brace indentation.

## Role In Callers

Local formatting constant for `JPrint`, `JPrintBuffered`, and preallocated formatted print paths through their shared print internals.

## Tests Observed

Focused JSON print tests cover formatted output behavior indirectly; no test targets this macro by name.

## Graph Links

- Graph node id: `macro:n_cjson.c:80:PRINT_TAB_CHARS`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none.

## Evidence Gaps

No unresolved source gaps for this node.
