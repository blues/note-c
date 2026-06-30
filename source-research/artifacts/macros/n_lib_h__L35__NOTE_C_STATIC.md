# `NOTE_C_STATIC`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:35`
- Declaration/interface: internal portability/testability macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: expands to `static` unless `NOTE_C_TEST` is defined; test builds include `test/include/test_static.h` and leave it empty.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: translation units including `n_lib.h`; CMake test target defines `NOTE_C_TEST`.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Default builds make internal helpers and globals translation-unit local with `static`. Test builds define `NOTE_C_TEST`, include `test/include/test_static.h`, and expose those helpers for focused unit tests and fakes.

## Important Boundaries

- Compile-time only; no runtime branch.
- Keeps portable SDK internals private by default while allowing test builds to mock or inspect normally static symbols.
- `NOTE_C_TEST` is set by the test target build configuration, not by production code.

## Direct Call Sites

`n_lib.h:31-35` defines the branch. `CMakeLists.txt:202-205` adds `NOTE_C_TEST` to the test target, and `test/include/test_static.h:7-10` documents/redefines the empty test behavior. Numerous internal functions and globals use `NOTE_C_STATIC` in source files.

## Role In Callers

Controls linkage of internal helpers such as CRC functions, hook state, I/O helpers, and cJSON internals. It does not change the helper bodies.

## Tests Observed

Focused test infrastructure depends on the empty test branch to reach normally static functions such as `_crcAdd` and `_crcError`.

## Graph Links

- Graph node id: `macro:n_lib.h:35:NOTE_C_STATIC`
- Structured graph: `inventory/dependency-graph.json`


## Evidence Gaps

No unresolved source gaps for this node.
