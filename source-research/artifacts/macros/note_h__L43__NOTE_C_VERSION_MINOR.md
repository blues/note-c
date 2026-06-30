# `NOTE_C_VERSION_MINOR`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:43`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_C_VERSION_MINOR`
- Macro aliases/wrappers: literal `6`.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, alias, attribute, or expression.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the minor component of the SDK version. `NOTE_C_VERSION` expands and stringizes this component into the public version string.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Referenced by the `NOTE_C_VERSION` macro expression.

## Role In Callers

Keeps the numeric version component separately available while feeding the string version used in user-agent data.

## Tests Observed

No focused test directly asserts the numeric version macro.

## Graph Links

- Graph node id: `macro:note.h:43:NOTE_C_VERSION_MINOR`
- Structured graph: `inventory/dependency-graph.json`
- Verified dependency edge from `NOTE_C_VERSION`.

## Evidence Gaps

No unresolved source gaps for this node.
