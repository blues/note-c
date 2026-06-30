# `N_CJSON_PUBLIC` (`n_cjson.h:138`)

## Status

Finalized. Source-inspected Windows import public API decoration branch.

## Identity

- Kind: macro
- Definition: `n_cjson.h:138`
- Declaration/interface: conditional macro definition in the cJSON public header
- Public/exported names: `N_CJSON_PUBLIC`
- Macro aliases/wrappers: one of five mutually exclusive conditional definitions of `N_CJSON_PUBLIC(type)`
- Signature/API shape: `#define N_CJSON_PUBLIC(type)   __declspec(dllimport) type __stdcall`
- Visibility: active on Win32 when `N_CJSON_IMPORT_SYMBOLS` is defined and hide/export branches are not selected
- Ownership/lifetime role: compile-time ABI decoration only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: Win32 `__declspec(dllimport)` and `__stdcall` support
- Data/type/global dependencies: preprocessor symbols `WIN32` and `N_CJSON_IMPORT_SYMBOLS`
- Artifact coverage: source-inspected artifact

## Behavior

When this branch is active, `N_CJSON_PUBLIC(type)` expands to `__declspec(dllimport) type __stdcall`. It marks cJSON API functions for DLL import and uses the stdcall convention.

## Important Boundaries

- Preconditions and assumptions: selected only by the surrounding preprocessor branch for Win32 import builds.
- Error/null/empty behavior: not a runtime construct.
- Ownership and lifetime: no runtime ownership.
- Concurrency/global state: none.
- Portability/platform constraints: Windows DLL-import branch; callers must match the library's export/import configuration.
- Performance shape: compile-time only.
- Security or parsing constraints: none.

## Direct Call Sites

Source search found broad `N_CJSON_PUBLIC(...)` use on cJSON declarations in `n_cjson.h` and matching definitions in `n_cjson.c`, plus one focused test fake declaration. This specific conditional branch is selected only by build flags, not by a separate call site.

## Role In Callers

Decorates the public cJSON API surface for Win32 clients importing cJSON symbols while preserving the function return type supplied as the macro argument.

## Tests Observed

No focused test directly exercises this build-flag branch. Existing C/C++ compilation covers the active branch for the current platform; Win32 import ABI behavior is not covered in the local focused tests.

## Graph Links

- Graph node id: `macro:n_cjson.h:138:N_CJSON_PUBLIC`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a compile-time decorator macro.

## Evidence Gaps

No unresolved source gaps for this node.
