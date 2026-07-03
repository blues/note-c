# `N_CJSON_PUBLIC` (`n_cjson.h:136`)

## Status

Finalized. Source-inspected Windows export public API decoration branch.

## Identity

- Kind: macro
- Definition: `n_cjson.h:136`
- Declaration/interface: conditional macro definition in the cJSON public header
- Public/exported names: `N_CJSON_PUBLIC`
- Macro aliases/wrappers: one of five mutually exclusive conditional definitions of `N_CJSON_PUBLIC(type)`
- Signature/API shape: `#define N_CJSON_PUBLIC(type)   __declspec(dllexport) type __stdcall`
- Visibility: active on Win32 when `N_CJSON_EXPORT_SYMBOLS` is defined and hidden symbols are not selected
- Ownership/lifetime role: compile-time ABI decoration only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: Win32 `__declspec(dllexport)` and `__stdcall` support
- Data/type/global dependencies: preprocessor symbols `WIN32` and `N_CJSON_EXPORT_SYMBOLS`; the header defaults to defining `N_CJSON_EXPORT_SYMBOLS` on Win32 when no hide/import/export symbol mode is set
- Artifact coverage: source-inspected artifact

## Behavior

When this branch is active, `N_CJSON_PUBLIC(type)` expands to `__declspec(dllexport) type __stdcall`. It marks cJSON API functions for DLL export and uses the stdcall convention.

## Important Boundaries

- Preconditions and assumptions: selected only by the surrounding preprocessor branch for Win32 export builds.
- Error/null/empty behavior: not a runtime construct.
- Ownership and lifetime: no runtime ownership.
- Concurrency/global state: none.
- Portability/platform constraints: Windows DLL-export branch; non-Windows builds use later definitions.
- Performance shape: compile-time only.
- Security or parsing constraints: none.

## Direct Call Sites

Source search found broad `N_CJSON_PUBLIC(...)` use on cJSON declarations in `n_cjson.h` and matching definitions in `n_cjson.c`, plus one focused test fake declaration. This specific conditional branch is selected only by build flags, not by a separate call site.

## Role In Callers

Decorates the public cJSON API surface for Win32 DLL-producing builds while preserving the function return type supplied as the macro argument.

## Tests Observed

No focused test directly exercises this build-flag branch. Existing C/C++ compilation covers the active branch for the current platform; Win32 export ABI behavior is not covered in the local focused tests.

## Graph Links

- Graph node id: `macro:n_cjson.h:136:N_CJSON_PUBLIC`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a compile-time decorator macro.

## Evidence Gaps

No unresolved source gaps for this node.
