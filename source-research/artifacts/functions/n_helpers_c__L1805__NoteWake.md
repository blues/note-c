# `NoteWake`

## Status

Finalized. Source-inspected public fixed-length wake-state restore wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1805`
- Declaration/interface: `note.h:1986`
- Signature/API shape: `bool NoteWake(int stateLen, void *state)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NoteWake`
- Macro aliases/wrappers: none
- Ownership/lifetime role: copies descriptor-owned payload into caller buffer and frees descriptor storage

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:1830:NotePayloadRetrieveAfterSleep` finalized in this slice
  - `function:n_helpers.c:2453:NotePayloadFree` finalized in this slice
- External/system dependencies: `memcpy`
- Data/type/global dependencies: stack `NotePayloadDesc`
- Artifact coverage: generated artifact exists

## Behavior

Creates a stack `NotePayloadDesc`, retrieves a sleep payload into it, and returns false if retrieval fails. If the decoded descriptor length does not equal `(uint32_t)stateLen`, frees the descriptor and returns false. On a length match, copies `stateLen` bytes from `desc.data` into caller `state`, frees the descriptor, and returns true.

## Important Boundaries

- Preconditions and assumptions: `state` must point to at least `stateLen` writable bytes when retrieval succeeds and lengths match.
- Error/null/empty behavior: retrieval failure returns false without cleanup because no descriptor ownership is established; wrong length frees descriptor and returns false.
- Ownership and lifetime: consumes the temporary descriptor through `NotePayloadFree` on all post-retrieval paths.
- Concurrency/global state: inherits request/time-cache/allocator behavior from `NotePayloadRetrieveAfterSleep`.
- Portability/platform constraints: state copy uses standard `memcpy`.
- Performance shape: one payload retrieval plus O(stateLen) copy on success.
- Security or parsing constraints: negative `stateLen` is cast to `uint32_t` for the length comparison and passed to `memcpy` only if it matches the descriptor length.

## Direct Call Sites

No in-repository production call sites were found beyond the definition and public declaration in `note.h:1986`.

## Role In Callers

This is the fixed-length convenience API above descriptor-level sleep payload retrieval.

## Tests Observed

Focused `test/src/NoteWake_test.cpp` covers retrieval failure, wrong decoded length cleanup, and successful state copy.

## Graph Links

- Graph node id: `function:n_helpers.c:1805:NoteWake`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NotePayloadRetrieveAfterSleep` and `NotePayloadFree`.

## Evidence Gaps

No unresolved source gaps for this node.
