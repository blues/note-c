# `JTIME`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `note.h:105`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `JTIME` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: `typedef JUINTEGER JTIME`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Aliases a standard integer/floating type or another SDK typedef.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines epoch-second time values returned by Notecard time/status/location helpers.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used by time base globals and functions such as `NoteTime`, `NoteTimeST`, `NoteTimeSet`, `NoteGetLocation`, and status helpers.

## Role In Callers

Provides the public scalar contract for JSON numeric or time APIs.

## Tests Observed

Focused time/location/status tests exercise JTIME-valued APIs.

## Graph Links

- Graph node id: `typedef:note.h:105:JTIME`
- Structured graph: `inventory/dependency-graph.json`
- Representative `uses_type` graph edges connect consuming APIs to this typedef.

## Evidence Gaps

No unresolved source gaps for this node.
