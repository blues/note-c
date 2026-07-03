# `N_CJSON_PUBLIC` (`n_cjson.h:134`)

## Status

Finalized. Source-inspected Windows hidden-symbol public API decoration branch.

## Identity

- Kind: macro
- Definition: `n_cjson.h:134`
- Declaration/interface: conditional macro definition in the cJSON public header
- Public/exported names: `N_CJSON_PUBLIC`
- Macro aliases/wrappers: one of five mutually exclusive conditional definitions of `N_CJSON_PUBLIC(type)`
- Signature/API shape: `#define N_CJSON_PUBLIC(type)   type __stdcall`
- Visibility: active on Win32 when `N_CJSON_HIDE_SYMBOLS` is defined
- Ownership/lifetime role: compile-time ABI decoration only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: Win32 `__stdcall` calling convention support
- Data/type/global dependencies: preprocessor symbols `WIN32` and `N_CJSON_HIDE_SYMBOLS`
- Artifact coverage: source-inspected artifact

## Behavior

When this branch is active, `N_CJSON_PUBLIC(type)` expands to `type __stdcall`. It keeps cJSON declarations callable with the Windows stdcall convention without adding import/export attributes.

## Important Boundaries

- Preconditions and assumptions: selected only by the surrounding preprocessor branch for Win32 hidden-symbol builds.
- Error/null/empty behavior: not a runtime construct.
- Ownership and lifetime: no runtime ownership.
- Concurrency/global state: none.
- Portability/platform constraints: Windows-only branch; incompatible compilers must provide `__stdcall` or avoid this configuration.
- Performance shape: compile-time only.
- Security or parsing constraints: none.

## Direct Call Sites

Source search found broad `N_CJSON_PUBLIC(...)` use on cJSON declarations in `n_cjson.h` and matching definitions in `n_cjson.c`, plus one focused test fake declaration. This specific conditional branch is selected only by build flags, not by a separate call site.

## Role In Callers

Decorates the public cJSON API surface for a hidden-symbol Win32 build while preserving the function return type supplied as the macro argument.

## Tests Observed

No focused test directly exercises this build-flag branch. Existing C/C++ compilation covers the active branch for the current platform; Win32 hidden-symbol ABI behavior is not covered in the local focused tests.

## Graph Links

- Graph node id: `macro:n_cjson.h:134:N_CJSON_PUBLIC`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a compile-time decorator macro.

## Evidence Gaps

No unresolved source gaps for this node.
