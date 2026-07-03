# `NoteMemAvailable`

## Status

Finalized. Source-inspected public allocator-probing memory helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:2399`
- Declaration/interface: `note.h:1111`
- Public/exported names: `NoteMemAvailable`
- Macro aliases/wrappers: none found
- Signature/API shape: `uint32_t NoteMemAvailable(void)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: temporarily allocates blocks from the configured SDK allocator, links them through an in-block header, then frees every allocated block before returning

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:241:_Malloc` finalized
  - `macro:n_lib.h:242:_Free` finalized
- External/system dependencies: configured platform allocator hooks behind `NoteMalloc` and `NoteFree`
- Data/type/global dependencies: static local `maxsize` initialized to `35000`; temporary `objHeader` chain stored inside allocated blocks
- Artifact coverage: source-inspected artifact

## Behavior

Initializes a local linked list of allocation records and loops from a static `maxsize` of `35000` down to `sizeof(objHeader)`, stepping by `sizeof(objHeader)`. For each size, repeatedly calls `_Malloc(i)` until allocation fails; each successful block is treated as an `objHeader`, linked onto the list, and records `length = i`. After probing all sizes, walks the linked list, sums every recorded allocation size into a `uint32_t`, frees each block with `_Free`, and returns the total.

## Important Boundaries

- Preconditions and assumptions: the configured allocator must return memory that is writable for at least `sizeof(objHeader)` when allocation succeeds. The helper assumes allocation probing is acceptable in the current runtime.
- Error/null/empty behavior: if every `_Malloc` call fails, returns zero. There is no direct error channel.
- Ownership and lifetime: every successful allocation is linked and then freed before return. The function writes its `objHeader` metadata into the allocated block, so allocated bytes are not preserved.
- Concurrency/global state: uses a static local `maxsize` value and global allocator hooks through `_Malloc`/`_Free`; no locking around allocator probing.
- Portability/platform constraints: behavior depends entirely on the configured allocator implementation and heap/fragmentation behavior; it does not query the Notecard over transport.
- Performance shape: intentionally allocates until failure across many sizes and can be expensive/disruptive; total work depends on available heap and allocator behavior.
- Security or parsing constraints: no parsing. Returned total can differ from true contiguous free space and can perturb allocator state through allocation/free churn.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1111` and the definition in `n_helpers.c:2399`. No production repository caller or focused test was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that want an allocator-based estimate of available memory.

## Tests Observed

No focused tests for `NoteMemAvailable` were found.

## Graph Links

- Graph node id: `function:n_helpers.c:2399:NoteMemAvailable`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_Malloc` and `_Free`.

## Evidence Gaps

No unresolved source gaps for this node.
