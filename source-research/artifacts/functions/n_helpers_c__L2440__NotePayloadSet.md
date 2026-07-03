# `NotePayloadSet`

## Status

Finalized. Source-inspected public payload descriptor initializer.

## Identity

- Kind: function
- Definition: `n_helpers.c:2440`
- Declaration/interface: `note.h:2140`
- Public/exported names: `NotePayloadSet`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NotePayloadSet(NotePayloadDesc *desc, uint8_t *buf, uint32_t buflen)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: records caller-provided buffer storage in a caller-owned `NotePayloadDesc`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: `NotePayloadDesc` fields `data`, `alloc`, and `length`; payload size constants live beside the type in `note.h`
- Artifact coverage: source-inspected artifact

## Behavior

Stores `buf` directly into `desc->data`, stores `buflen` into `desc->alloc`, and stores `buflen` into `desc->length`. It performs no allocation, copy, validation, error reporting, or ownership transfer beyond updating those descriptor fields.

## Important Boundaries

- Preconditions and assumptions: `desc` must be non-null. The function dereferences it unconditionally.
- Error/null/empty behavior: `buf` may be null and is still stored as-is. A null buffer with a nonzero length is not rejected.
- Ownership and lifetime: the descriptor aliases the caller-provided buffer. Later descriptor helpers such as `NotePayloadFree` free `desc->data` when non-null, so callers must only use this initializer with storage that is compatible with the SDK free hook if the descriptor will be freed or grown by note-c payload helpers.
- Concurrency/global state: no global state or hooks are touched directly.
- Portability/platform constraints: plain pointer and `uint32_t` field assignments only.
- Performance shape: constant-time three-field assignment.
- Security or parsing constraints: no length validation and no buffer provenance check.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:2140` and the definition in `n_helpers.c:2440`. No production repository caller, test caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that want to initialize a `NotePayloadDesc` around an existing payload buffer or an empty descriptor state.

## Tests Observed

No focused tests for `NotePayloadSet` were found. Payload tests exercise other descriptor helpers but do not call this function.

## Graph Links

- Graph node id: `function:n_helpers.c:2440:NotePayloadSet`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
