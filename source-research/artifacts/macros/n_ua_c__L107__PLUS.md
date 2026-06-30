# `PLUS`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_ua.c:107`
- Declaration/interface: translation-unit-local compiler-string macro inside `NoteUserAgent`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal string `" c++"` under `__cplusplus`.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: `NoteUserAgent` compilation under C++.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Appends a C++ marker to the compiler name embedded in the user-agent JSON when the translation unit is compiled as C++.

## Important Boundaries

- Compile-time branch on `__cplusplus`.
- Undefined/redefined only inside `n_ua.c` before compiler-specific user-agent strings.
- Has no effect when user-agent support is disabled by low-memory configuration.

## Direct Call Sites

Used by `NoteUserAgent` in compiler string construction for IAR, clang, GCC, MSVC, STDC, and fallback branches.

## Role In Callers

Carries language-mode information into the diagnostic user-agent payload.

## Tests Observed

Focused `NoteUserAgent_test.cpp` checks compiler field content for the active test compiler; it does not separately force the C++ marker branch.

## Graph Links

- Graph node id: `macro:n_ua.c:107:PLUS`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `NoteUserAgent`.

## Evidence Gaps

No unresolved source gaps for this node.
