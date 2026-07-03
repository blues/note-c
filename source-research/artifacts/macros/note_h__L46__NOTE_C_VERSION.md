# `NOTE_C_VERSION`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:46`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_C_VERSION`
- Macro aliases/wrappers: expression `NOTE_C_STRINGIZE(MAJOR) "." NOTE_C_STRINGIZE(MINOR) "." NOTE_C_STRINGIZE(PATCH)`, currently `"2.6.1"`.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Depends on version component macros and `NOTE_C_STRINGIZE`.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Builds the public SDK version string through two-stage stringization. `n_ua.c` embeds it in the default user-agent string as `"note-c " NOTE_C_VERSION`.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Referenced by `n_ua.c:19` default `n_agent` initialization.

## Role In Callers

Connects header version constants to runtime user-agent identity.

## Tests Observed

Focused `NoteUserAgent_test.cpp` checks the user-agent agent field but does not independently assert the literal version.

## Graph Links

- Graph node id: `macro:note.h:46:NOTE_C_VERSION`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge from `NoteUserAgent` and macro dependency edges to version components.

## Evidence Gaps

No unresolved source gaps for this node.
