# `NOTE_C_DEPRECATED`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:21`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_C_DEPRECATED`
- Macro aliases/wrappers: MSVC branch `__declspec(deprecated)`.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Compile branch depends on `_MSC_VER`.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Annotates deprecated public declarations when compiling with MSVC. The fallback branch is empty and defines `NOTE_C_NO_DEPRECATED_ATTR`, but no separate graph node exists for that fallback macro.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Used on the same deprecated public declarations as the GCC/Clang branch.

## Role In Callers

Provides MSVC deprecation warnings without changing ABI or runtime behavior.

## Tests Observed

No focused test asserts compiler deprecation diagnostics.

## Graph Links

- Graph node id: `macro:note.h:21:NOTE_C_DEPRECATED`
- Structured graph: `inventory/dependency-graph.json`
- Verified `annotates_decl` edges to the deprecated public functions.

## Evidence Gaps

No unresolved source gaps for this node.
