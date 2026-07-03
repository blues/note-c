# `NoteResponseErrorContains`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:480`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NoteResponseErrorContains`
- Macro aliases/wrappers: wrapper `(JContainsString(rsp, "err", errstr))`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Wrapper dependency on `JContainsString` and fixed key literal `"err"`.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Tests whether a response object has an `"err"` field containing the requested substring by delegating to `JContainsString`.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers can use this value/expression wherever `note.h` is included.
- Exact source search distinguishes macro use from related literal or function use.

## Direct Call Sites

No production C source uses this public macro; focused tests call it directly in `NoteTransaction_test.cpp`.

## Role In Callers

Public convenience for checking tagged Notecard errors while preserving cJSON helper semantics.

## Tests Observed

Focused `NoteTransaction_test.cpp` checks the macro against an `{io}` response.

## Graph Links

- Graph node id: `macro:note.h:480:NoteResponseErrorContains`
- Structured graph: `inventory/dependency-graph.json`
- Verified `expands_to` edge to `JContainsString`.

## Evidence Gaps

No unresolved source gaps for this node.
