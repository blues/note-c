# `NP_SEGHDR_LEN`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:2104`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NP_SEGHDR_LEN`
- Macro aliases/wrappers: expression `(NP_SEGTYPE_LEN + NP_SEGLEN_LEN)`, effectively `8` bytes.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Depends on `NP_SEGTYPE_LEN` and `NP_SEGLEN_LEN`.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines total segment header size. Payload add includes it in allocation/length accounting; payload find uses it for loop bounds, payload data pointer, and segment skipping.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Used by `NotePayloadAddSegment` and `NotePayloadFindSegment`.

## Role In Callers

Keeps header-size arithmetic consistent between writer and scanner.

## Tests Observed

Focused `NotePayload_test.cpp` covers add/find/get, duplicate ID, missing ID, and reallocation behavior.

## Graph Links

- Graph node id: `macro:note.h:2104:NP_SEGHDR_LEN`
- Structured graph: `inventory/dependency-graph.json`
- Verified dependency edges to segment type/length constants and uses edges to payload functions.

## Evidence Gaps

No unresolved source gaps for this node.
