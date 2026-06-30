# `NotePayloadFree`

## Status

Finalized. Source-inspected public payload descriptor cleanup helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:2453`
- Declaration/interface: `note.h:2146`
- Signature/API shape: `void NotePayloadFree(NotePayloadDesc *desc)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NotePayloadFree`
- Macro aliases/wrappers: none
- Ownership/lifetime role: releases descriptor-owned payload buffer and resets descriptor fields

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:242:_Free` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: mutates caller-owned `NotePayloadDesc`
- Artifact coverage: generated artifact exists

## Behavior

If `desc->data` is non-null, frees it with `_Free`. Then sets `desc->data = NULL`, `desc->alloc = 0`, and `desc->length = 0`.

## Important Boundaries

- Preconditions and assumptions: `desc` must be non-null and `desc->data`, when non-null, must be compatible with the SDK free hook.
- Error/null/empty behavior: null `desc->data` is accepted; null `desc` would dereference.
- Ownership and lifetime: consumes descriptor ownership of `data` and leaves the descriptor empty.
- Concurrency/global state: free uses hook-backed `_Free`.
- Portability/platform constraints: allocation ownership stays behind SDK hooks.
- Performance shape: O(1) plus allocator free cost.
- Security or parsing constraints: no validation of pointer provenance.

## Direct Call Sites

Exact source search found production callers:

- `n_helpers.c:1813` and `n_helpers.c:1817` in `NoteWake`.

## Role In Callers

This is the cleanup API for descriptors filled by payload helpers and for ownership transferred by `NotePayloadSet`.

## Tests Observed

Focused tests call this helper after payload retrieval and segment-manipulation scenarios (`NotePayloadRetrieveAfterSleep_test.cpp`, `NotePayload_test.cpp`).

## Graph Links

- Graph node id: `function:n_helpers.c:2453:NotePayloadFree`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_Free`.

## Evidence Gaps

No unresolved source gaps for this node.
