# `NoteAddBodyToObject`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:323`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NoteAddBodyToObject`
- Macro aliases/wrappers: wrapper `JAddItemToObject(a, "body", b)`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Wrapper dependency on `JAddItemToObject` and fixed key literal `"body"`.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Adds an existing `J` item under the fixed key `"body"` by expanding to `JAddItemToObject`.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers can use this value/expression wherever `note.h` is included.
- Exact source search distinguishes macro use from related literal or function use.

## Direct Call Sites

Exact source search found no production `NoteAddBodyToObject` call sites outside the public macro definition.

## Role In Callers

Public compatibility convenience for adding request/response bodies; ownership follows `JAddItemToObject` semantics.

## Tests Observed

No focused test directly invokes `NoteAddBodyToObject`.

## Graph Links

- Graph node id: `macro:note.h:323:NoteAddBodyToObject`
- Structured graph: `inventory/dependency-graph.json`
- Verified `expands_to` edge to `JAddItemToObject`.

## Evidence Gaps

No unresolved source gaps for this node.
