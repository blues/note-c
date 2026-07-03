# `NP_SEGLEN_LEN`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:2103`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NP_SEGLEN_LEN`
- Macro aliases/wrappers: expression `sizeof(uint32_t)`, effectively `4` bytes.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, alias, attribute, or expression.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the raw length-field width copied into each payload segment header by `NotePayloadAddSegment`.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Used by `NotePayloadAddSegment`; `NotePayloadFindSegment` reads the same field with `sizeof(len)`.

## Role In Callers

Defines the second field of the binary segmented-payload header. The serialized length is native-endian because source uses raw `memcpy` of a `uint32_t`.

## Tests Observed

Focused `NotePayload_test.cpp` covers payload add/find/get behavior.

## Graph Links

- Graph node id: `macro:note.h:2103:NP_SEGLEN_LEN`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `NotePayloadAddSegment`.

## Evidence Gaps

No unresolved source gaps for this node.
