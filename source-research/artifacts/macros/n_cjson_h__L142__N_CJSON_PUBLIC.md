# `N_CJSON_PUBLIC` (`n_cjson.h:142`)

## Status

Finalized. Source-inspected non-Windows visibility public API decoration branch.

## Identity

- Kind: macro
- Definition: `n_cjson.h:142`
- Declaration/interface: conditional macro definition in the cJSON public header
- Public/exported names: `N_CJSON_PUBLIC`
- Macro aliases/wrappers: one of five mutually exclusive conditional definitions of `N_CJSON_PUBLIC(type)`
- Signature/API shape: `#define N_CJSON_PUBLIC(type)   __attribute__((visibility("default"))) type`
- Visibility: active on non-Windows builds when GCC/SunPro-style visibility attributes are available and `N_CJSON_API_VISIBILITY` is defined
- Ownership/lifetime role: compile-time ABI decoration only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: compiler support for `__attribute__((visibility("default")))`
- Data/type/global dependencies: preprocessor symbols `__GNUC__`, `__SUNPRO_CC`, `__SUNPRO_C`, and `N_CJSON_API_VISIBILITY`
- Artifact coverage: source-inspected artifact

## Behavior

When this branch is active, `N_CJSON_PUBLIC(type)` expands to `__attribute__((visibility("default"))) type`. It marks cJSON API functions as default-visibility symbols in builds that otherwise hide symbols.

## Important Boundaries

- Preconditions and assumptions: selected only for non-Windows compiler/flag combinations that request explicit API visibility.
- Error/null/empty behavior: not a runtime construct.
- Ownership and lifetime: no runtime ownership.
- Concurrency/global state: none.
- Portability/platform constraints: non-Windows compiler-attribute branch.
- Performance shape: compile-time only.
- Security or parsing constraints: none.

## Direct Call Sites

Source search found broad `N_CJSON_PUBLIC(...)` use on cJSON declarations in `n_cjson.h` and matching definitions in `n_cjson.c`, plus one focused test fake declaration. This specific conditional branch is selected only by build flags, not by a separate call site.

## Role In Callers

Decorates the public cJSON API surface for hidden-symbol non-Windows builds that still need selected API symbols visible.

## Tests Observed

No focused test directly exercises this build-flag branch. Existing C/C++ compilation covers the active branch for the current platform only when `N_CJSON_API_VISIBILITY` is present; explicit visibility-attribute behavior is not covered in the local focused tests.

## Graph Links

- Graph node id: `macro:n_cjson.h:142:N_CJSON_PUBLIC`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a compile-time decorator macro.

## Evidence Gaps

No unresolved source gaps for this node.
