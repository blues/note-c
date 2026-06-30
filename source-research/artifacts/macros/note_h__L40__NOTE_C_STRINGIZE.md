# `NOTE_C_STRINGIZE`

## Status

Finalized. Source-inspected public two-step stringize macro and exact source uses.

## Identity

- Kind: macro
- Definition: `note.h:40`
- Declaration/interface: public header macro in `note.h`
- Public/exported names: `NOTE_C_STRINGIZE`
- Macro aliases/wrappers: expands to adjacent helper `_NOTE_C_STRINGIZE(x)` defined at `note.h:39`
- Signature/API shape: `NOTE_C_STRINGIZE(x)`
- Visibility: available to translation units including `note.h`
- Ownership/lifetime role: compile-time token-to-string-literal helper; no runtime ownership

## Dependency Status

- Project-local dependencies: adjacent helper macro `_NOTE_C_STRINGIZE(x) #x` source-inspected; generated graph does not model it as an outgoing edge
- External/system dependencies: preprocessor stringification
- Data/type/global dependencies: compile-time macro arguments only; no runtime state
- Artifact coverage: definition, adjacent helper, source uses, and generated graph node inspected

## Behavior

Provides the second expansion layer for preprocessor stringification. `NOTE_C_STRINGIZE(x)` expands `x` first, then delegates to `_NOTE_C_STRINGIZE(x)`, whose `#x` operator turns the expanded token sequence into a string literal.

## Important Boundaries

- Preconditions and assumptions: argument must be valid preprocessor tokens in the call site.
- Error/null/empty behavior: no runtime error channel; invalid macro input fails at preprocessing/compilation.
- Ownership and lifetime: produces string literals at compile time; no allocation.
- Concurrency/global state: no runtime state.
- Portability/platform constraints: standard C/C++ preprocessor behavior; used in public headers and C sources.
- Performance shape: compile-time only.
- Security or parsing constraints: stringized values may be embedded in diagnostics/version/compiler strings but no runtime parsing occurs here.

## Direct Call Sites

Exact source search found direct uses:

- `note.h:46` builds `NOTE_C_VERSION` from `NOTE_C_VERSION_MAJOR`, `NOTE_C_VERSION_MINOR`, and `NOTE_C_VERSION_PATCH`.
- `note.h:983` embeds `__LINE__` in `NOTE_C_LOG_FILE_AND_LINE` when `NOTE_C_LOG_SHOW_FILE_AND_LINE` is enabled.
- `n_cjson.c:112` `JVersion` builds the CJSON version string from `N_CJSON_VERSION_*` macros.
- `n_ua.c:113` and `n_ua.c:115` build IAR compiler strings from `__VER__` in `NoteUserAgent` compiler-detection branches.

## Role In Callers

Callers use this macro where numeric/version/compiler preprocessor symbols must become string literals after macro expansion. It is a compile-time support macro for version constants, optional file/line logging, CJSON version reporting, and user-agent compiler metadata.

## Tests Observed

No focused tests were found for this macro. Source inspection is sufficient because behavior is compile-time preprocessor expansion.

## Graph Links

- Graph node id: `macro:note.h:40:NOTE_C_STRINGIZE`
- Structured graph: `inventory/dependency-graph.json`
- Generated incoming edge observed from `function:n_cjson.c:110:JVersion`; additional preprocessor uses in `note.h` and `n_ua.c` are source-inspected call sites.

## Evidence Gaps

No unresolved source gaps for this node. The adjacent helper `_NOTE_C_STRINGIZE` remains a separate preliminary artifact because this active slice tracks the public macro node only.
