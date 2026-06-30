# `NotePayloadSaveAndSleep`

## Status

Finalized. Source-inspected public payload-save sleep helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1708`
- Declaration/interface: `note.h:2124`; `NotePayloadDesc` type at `note.h:2109`
- Public/exported names: `NotePayloadSaveAndSleep`
- Macro aliases/wrappers: none found
- Signature/API shape: `bool NotePayloadSaveAndSleep(NotePayloadDesc *desc, uint32_t seconds, const char *modes)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: borrows caller-owned payload data, allocates a temporary base64 string when needed, passes it synchronously to `NoteSleep`, then frees it

## Dependency Status

- Project-local dependencies:
  - `function:n_b64.c:169:JB64EncodeLen` finalized
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_b64.c:174:JB64Encode` finalized
  - `function:n_helpers.c:1754:NoteSleep` finalized
  - `macro:n_lib.h:242:_Free` finalized
- External/system dependencies: Notecard `card.attn` command path through `NoteSleep`
- Data/type/global dependencies: `NotePayloadDesc` fields `data`, `alloc`, and `length`; transitive allocation, logging, JSON, and request hooks through callees
- Artifact coverage: source-inspected artifact

## Behavior

Initializes a temporary base64 pointer to null. If `desc->data` is non-null and `desc->length` is non-zero, computes the base64 encoded length with `JB64EncodeLen`, allocates that many bytes with `_Malloc`, returns false on allocation failure, and encodes `desc->data` into the temporary buffer with `JB64Encode`. It then calls `NoteSleep(stateB64, seconds, modes)`, frees the temporary buffer when one was allocated, and returns the `NoteSleep` result. When the descriptor has null data or zero length, it calls `NoteSleep(NULL, seconds, modes)` without allocating.

## Important Boundaries

- Preconditions and assumptions: `desc` must be non-null because the function reads `desc->data` before any guard. `desc->alloc` is not consulted; the function trusts `desc->length` bytes are readable from `desc->data`.
- Error/null/empty behavior: null `desc->data` or zero `desc->length` is treated as no payload and still delegates to `NoteSleep`. Temporary allocation failure returns false without calling `NoteSleep`. `NoteSleep` false propagates after freeing any temporary buffer.
- Ownership and lifetime: caller retains ownership of `desc` and `desc->data`. The base64 buffer is owned by this function and freed before return. `NoteSleep` attaches a borrowed string reference and consumes/deletes its command synchronously before returning, so the temporary buffer remains valid for its use.
- Concurrency/global state: no local static state; transitive allocation, logging, JSON, and request behavior use global SDK hook layers.
- Portability/platform constraints: portable C helper; platform-specific behavior is behind allocator/request hooks.
- Performance shape: O(payload length) encode plus one allocation/free only when payload data and length are both present; otherwise a direct `NoteSleep` delegation.
- Security or parsing constraints: no local bounds check against `desc->alloc`; very large `length` values rely on finalized base64 length/allocation behavior.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:2124`, the definition in `n_helpers.c:1708`, and focused tests. No production repository caller was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that want to save an optional binary payload before putting the Notecard/host attention path into sleep mode.

## Tests Observed

Focused tests in `test/src/NotePayloadSaveAndSleep_test.cpp` fake `NoteMalloc` and `NoteSleep`. Covered cases: valid payload success, null payload data, zero payload length, allocation failure, and `NoteSleep` failure propagation. `test/src/NoteSleep_test.cpp` separately covers the delegated mode-string behavior. Gaps include null descriptor, exact base64 payload contents passed to `NoteSleep`, `desc->alloc` mismatch, very large length handling, and downstream JSON add failures inside `NoteSleep`.

## Graph Links

- Graph node id: `function:n_helpers.c:1708:NotePayloadSaveAndSleep`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JB64EncodeLen`, `_Malloc`, `JB64Encode`, `NoteSleep`, and `_Free`.

## Evidence Gaps

No unresolved source gaps for this node.
