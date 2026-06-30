# `NP_SEGTYPE_LEN`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:2102`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NP_SEGTYPE_LEN`
- Macro aliases/wrappers: literal `4`.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, alias, attribute, or expression.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the segment type identifier width. Payload add copies exactly four type bytes; payload find compares exactly four bytes and reads length immediately after those bytes.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Used by `NotePayloadAddSegment`, `NotePayloadFindSegment`, and the public payload function parameter declarations.

## Role In Callers

Defines the first field of the binary segmented-payload header.

## Tests Observed

Focused `NotePayload_test.cpp` covers add/find/get, duplicate IDs, missing IDs, and reallocation behavior.

## Graph Links

- Graph node id: `macro:note.h:2102:NP_SEGTYPE_LEN`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to payload add/find functions.

## Evidence Gaps

No unresolved source gaps for this node.
