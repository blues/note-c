# `NoteNewBody`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:322`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NoteNewBody`
- Macro aliases/wrappers: alias to `JCreateObject`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Alias dependency on `JCreateObject`.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Creates an empty JSON body object by expanding directly to the cJSON object constructor.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers can use this value/expression wherever `note.h` is included.
- Exact source search distinguishes macro use from related literal or function use.

## Direct Call Sites

Exact source search found no production `NoteNewBody` call sites outside the public macro definition.

## Role In Callers

Public compatibility convenience for body creation; behavior is exactly `JCreateObject` when used.

## Tests Observed

No focused test directly invokes `NoteNewBody`.

## Graph Links

- Graph node id: `macro:note.h:322:NoteNewBody`
- Structured graph: `inventory/dependency-graph.json`
- Verified `expands_to` edge to `JCreateObject`.

## Evidence Gaps

No unresolved source gaps for this node.
