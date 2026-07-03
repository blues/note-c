# `JINTEGER_MAX`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:95`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `JINTEGER_MAX`
- Macro aliases/wrappers: alias to `INT64_MAX`.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Aliases standard integer limit macro from `<stdint.h>`/`<stdint>` family.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Bounds the signed 64-bit `JINTEGER` range. cJSON number parsing and creation saturate `valueint` to this bound when the parsed/created `JNUMBER` exceeds the signed integer range.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Used by `_parse_number`, `JSetNumberHelper`, and `JCreateNumber`; `JINTEGER_MIN` is also used by `JItoA` to avoid signed overflow when converting the minimum value.

## Role In Callers

Preserves predictable integer saturation while JSON numbers remain represented as `JNUMBER` for numeric value.

## Tests Observed

Focused `JItoA_test.cpp` and `JSON_number_handling_test.cpp` cover min/max conversion and saturation behavior.

## Graph Links

- Graph node id: `macro:note.h:95:JINTEGER_MAX`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to cJSON number paths.

## Evidence Gaps

No unresolved source gaps for this node.
