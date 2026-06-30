# `typedef_2109`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `note.h:2109`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `typedef_2109` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: anonymous struct typedef `NotePayloadDesc` with `data`, `alloc`, and `length` fields
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Public payload descriptor struct.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines caller-visible segmented payload descriptor state. Payload functions set, allocate/free, append, find, and copy segments through this descriptor.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used by `NotePayloadSet`, `NotePayloadFree`, `NotePayloadAddSegment`, `NotePayloadFindSegment`, `NotePayloadGetSegment`, `NotePayloadSaveAndSleep`, and `NotePayloadRetrieveAfterSleep`.

## Role In Callers

Owns or references payload storage depending on setup; allocation/free behavior is managed by payload helper functions.

## Tests Observed

Focused `NotePayload_test.cpp` covers segment add/find/get, missing/duplicate behavior, and reallocation.

## Graph Links

- Graph node id: `typedef:note.h:2109:typedef_2109`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_type` edges to payload functions.

## Evidence Gaps

No unresolved source gaps for this node.
