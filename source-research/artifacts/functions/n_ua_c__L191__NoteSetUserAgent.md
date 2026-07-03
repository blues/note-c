# `NoteSetUserAgent`

## Status

Finalized. Source-inspected public user-agent string setter.

## Identity

- Kind: function
- Definition: `n_ua.c:191`
- Declaration/interface: `note.h:902`
- Public/exported names: `NoteSetUserAgent`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetUserAgent(char *agent)`
- Visibility: public note-c API when `n_ua.c` is built; excluded from low-memory builds by source/build configuration
- Ownership/lifetime role: stores caller-provided string pointer for later user-agent JSON construction

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes file-static `n_agent`; later read by `NoteUserAgent`
- Artifact coverage: source-inspected artifact

## Behavior

Assigns `n_agent = agent`. It does not copy, validate, or allocate the string.

## Important Boundaries

- Preconditions and assumptions: non-null `agent`, when provided, must remain valid until user-agent construction copies it.
- Error/null/empty behavior: null is accepted and stored; later JSON field insertion can fail or suppress data depending on consumer behavior.
- Ownership and lifetime: caller owns the pointed-to string; note-c stores only the pointer.
- Concurrency/global state: writes shared file-static state without locking.
- Portability/platform constraints: implementation exists only outside `NOTE_C_LOW_MEM` builds.
- Performance shape: one pointer assignment.
- Security or parsing constraints: no string validation or lifetime protection.

## Direct Call Sites

Production-only exact search found only the public declaration in `note.h:902` and the definition in `n_ua.c:191`. Focused tests call it from `test/src/NoteUserAgent_test.cpp`. No production repository caller, wrapper, or alias was found.

## Role In Callers

No production repository caller role. Public setter for downstream libraries to stamp their agent string into `NoteUserAgent` output.

## Tests Observed

Focused tests in `test/src/NoteUserAgent_test.cpp` set the agent and verify `NoteUserAgent()` emits it. Gaps include null agent, pointer lifetime after caller buffer mutation, and low-memory build API behavior.

## Graph Links

- Graph node id: `function:n_ua.c:191:NoteSetUserAgent`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
