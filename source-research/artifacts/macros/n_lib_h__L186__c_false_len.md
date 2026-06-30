# `c_false_len`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:186`
- Declaration/interface: shared internal string-length macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `5`, paired with `c_false = "false"`.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: translation units including `n_lib.h`.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Provides the compile-time byte length for the paired global constant string declared in `n_lib.h` and defined in `n_const.c`. Used by `_parse_value` to match `false` and by `_print_value` to allocate/copy the printed token.

## Important Boundaries

- Compile-time literal; no storage or ownership.
- Must stay synchronized with the paired `c_*` string definition in `n_const.c`.
- Exact search separated `_len` use from paired string use.

## Direct Call Sites

Used by `_parse_value` to match `false` and by `_print_value` to allocate/copy the printed token.

## Role In Callers

Avoids repeated runtime length computation for shared strings where callers need exact token lengths.

## Tests Observed

Focused cJSON parse/print tests exercise these JSON literal tokens.

## Graph Links

- Graph node id: `macro:n_lib.h:186:c_false_len`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to `_parse_value` and `_print_value`.

## Evidence Gaps

No unresolved source gaps for this node.
