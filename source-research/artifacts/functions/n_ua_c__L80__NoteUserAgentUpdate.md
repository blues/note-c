# `NoteUserAgentUpdate`

## Status

Finalized. Source-inspected weak user-agent extension hook.

## Identity

- Kind: function
- Definition: `n_ua.c:80`
- Declaration/interface: weak function outside MSVC, normal function under MSVC
- Signature/API shape: `void NoteUserAgentUpdate(J *ua)`
- Visibility: public override hook when user-agent support is compiled
- Public/exported names: `NoteUserAgentUpdate`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Default implementation ignores the passed user-agent object. Higher-level code may override the weak symbol outside MSVC.

## Important Boundaries

- Preconditions and assumptions: compiled only when user-agent support is included.
- Error/null/empty behavior: default implementation has no effect.
- Ownership and lifetime: does not allocate or retain the object.
- Concurrency/global state: no global state in default implementation.
- Performance shape: O(1).
- Portability/platform constraints: weak-symbol override is not used for MSVC.

## Direct Call Sites

Exact source search found `NoteUserAgent` as the production caller.

## Role In Callers

`NoteUserAgent` calls this hook after populating built-in fields so external code can add fields.

## Tests Observed

`NoteUserAgent_test.cpp` provides test-side override evidence for user-agent customization.

## Graph Links

- Graph node id: `function:n_ua.c:80:NoteUserAgentUpdate`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
