# `NOTE_C_DEPRECATED`

## Status

Finalized. Source-inspected definition, branch behavior, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:19`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_C_DEPRECATED`
- Macro aliases/wrappers: GCC/Clang branch `__attribute__((__deprecated__))`.
- Signature/API shape: preprocessor literal, alias, expression, attribute, or wrapper macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Compile branch depends on `__GNUC__ || __clang__`.
- External/system dependencies: C compiler/preprocessor branch or standard integer/limit macros where noted.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Annotates deprecated public declarations when compiling with GCC-compatible or Clang-compatible compilers. The function implementations remain ordinary functions; only declarations carry the compiler warning attribute.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers see this value when including `note.h`.
- Branch behavior depends on preprocessor configuration where noted.

## Direct Call Sites

Used on `NoteSuspendTransactionDebug`, `NoteResumeTransactionDebug`, and `NoteTurboIO` declarations in `note.h`.

## Role In Callers

Provides portable deprecation warnings without changing ABI or runtime behavior.

## Tests Observed

No focused test asserts compiler deprecation diagnostics.

## Graph Links

- Graph node id: `macro:note.h:19:NOTE_C_DEPRECATED`
- Structured graph: `inventory/dependency-graph.json`
- Verified `annotates_decl` edges to the deprecated public functions.

## Evidence Gaps

No unresolved source gaps for this node.
