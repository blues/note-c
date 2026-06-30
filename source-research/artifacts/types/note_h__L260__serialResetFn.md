# `serialResetFn`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: typedef
- Definition: `note.h:260`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `serialResetFn` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: `bool (*)(void)`
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Public function pointer typedef.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the public platform hook callback type for the serial reset hook. Hook setter/getter APIs store or return function pointers of this type; wrapper functions dispatch through the configured hook when set.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

NoteSetFnSerial/NoteSetFnSerialDefault/NoteGetFnSerial and serial reset dispatch

## Role In Callers

Keeps platform-specific behavior behind typed hooks so the portable C SDK core does not depend on platform APIs.

## Tests Observed

Focused hook tests exercise setter/getter and dispatch behavior for these callback families.

## Graph Links

- Graph node id: `typedef:note.h:260:serialResetFn`
- Structured graph: `inventory/dependency-graph.json`


## Evidence Gaps

No unresolved source gaps for this node.
