# `NoteGetBody`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:324`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NoteGetBody`
- Macro aliases/wrappers: wrapper `JGetObject(a, "body")`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Wrapper dependency on `JGetObject` and fixed key literal `"body"`.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Fetches the `"body"` object field by expanding to `JGetObject`.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers can use this value/expression wherever `note.h` is included.
- Exact source search distinguishes macro use from related literal or function use.

## Direct Call Sites

Exact source search found no production `NoteGetBody` call sites outside the public macro definition.

## Role In Callers

Public compatibility convenience for retrieving response/request bodies; null/missing handling follows `JGetObject`.

## Tests Observed

No focused test directly invokes `NoteGetBody`.

## Graph Links

- Graph node id: `macro:note.h:324:NoteGetBody`
- Structured graph: `inventory/dependency-graph.json`
- Verified `expands_to` edge to `JGetObject`.

## Evidence Gaps

No unresolved source gaps for this node.
