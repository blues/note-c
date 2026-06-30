# `JGetObjectItemName`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:1440`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `JGetObjectItemName`
- Macro aliases/wrappers: field accessor `(j->string)`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Direct struct-field access; no project-local function callee.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Returns the object item key string by directly reading the `string` member of the passed `J` pointer.

## Important Boundaries

- No null guard; `j` is evaluated directly.
- Exposes the existing `J` item key pointer; it does not allocate or copy.
- Lifetime follows the owning `J` object.

## Direct Call Sites

Exact source search found no production `JGetObjectItemName` call sites outside the public macro definition.

## Role In Callers

Public convenience accessor for cJSON object item names; caller must pass a non-null `J *` with a valid `string` member.

## Tests Observed

No focused test directly invokes `JGetObjectItemName`.

## Graph Links

- Graph node id: `macro:note.h:1440:JGetObjectItemName`
- Structured graph: `inventory/dependency-graph.json`


## Evidence Gaps

No unresolved source gaps for this node.
