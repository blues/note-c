# `TINT64`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:2187`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `TINT64`
- Macro aliases/wrappers: literal `18`
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Public template constant for `NoteTemplate`; encoded by caller through cJSON number helpers.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

8-byte signed integer template code; support depends on platform and Notecard behavior. `NoteTemplate` attaches and sends the caller-owned body unchanged.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Exact production source search found no in-repo direct macro use beyond the public definition. `NoteTemplate` is the production API that sends template bodies supplied by callers.

## Role In Callers

Defines stable public codes for callers building `note.template` request bodies.

## Tests Observed

Focused `NoteTemplate_test.cpp` covers request construction/success/failure, but no test asserts this macro value directly.

## Graph Links

- Graph node id: `macro:note.h:2187:TINT64`
- Structured graph: `inventory/dependency-graph.json`
- Recorded `public_template_constant_for` edge to `NoteTemplate` and caller-encoding edge to the relevant cJSON add helper.

## Evidence Gaps

No unresolved source gaps for this node.
