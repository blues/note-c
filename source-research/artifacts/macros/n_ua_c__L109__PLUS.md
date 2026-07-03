# `PLUS`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_ua.c:109`
- Declaration/interface: translation-unit-local compiler-string macro inside `NoteUserAgent`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal empty string under non-`__cplusplus` builds.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: `NoteUserAgent` compilation as C.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Leaves the compiler name unmarked when the translation unit is compiled as C.

## Important Boundaries

- Compile-time branch opposite the C++ marker.
- Used in every compiler-specific compiler string branch.
- Has no effect when user-agent support is disabled by low-memory configuration.

## Direct Call Sites

Used by `NoteUserAgent` in compiler string construction for IAR, clang, GCC, MSVC, STDC, and fallback branches.

## Role In Callers

Keeps C user-agent compiler strings compact while sharing the same construction code as the C++ branch.

## Tests Observed

Focused `NoteUserAgent_test.cpp` checks compiler field content for the active test compiler.

## Graph Links

- Graph node id: `macro:n_ua.c:109:PLUS`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `NoteUserAgent`.

## Evidence Gaps

No unresolved source gaps for this node.
