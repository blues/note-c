# `N_CJSON_PUBLIC` (`n_cjson.h:144`)

## Status

Finalized. Source-inspected default public API decoration branch.

## Identity

- Kind: macro
- Definition: `n_cjson.h:144`
- Declaration/interface: conditional macro definition in the cJSON public header
- Public/exported names: `N_CJSON_PUBLIC`
- Macro aliases/wrappers: one of five mutually exclusive conditional definitions of `N_CJSON_PUBLIC(type)`
- Signature/API shape: `#define N_CJSON_PUBLIC(type) type`
- Visibility: active on non-Windows builds without explicit cJSON API visibility attributes
- Ownership/lifetime role: compile-time type passthrough only

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: absence of the earlier selected Win32 or non-Windows visibility branches
- Artifact coverage: source-inspected artifact

## Behavior

When this branch is active, `N_CJSON_PUBLIC(type)` expands to `type` unchanged. It leaves cJSON declarations and definitions undecorated.

## Important Boundaries

- Preconditions and assumptions: selected by the surrounding non-Windows fallback preprocessor branch.
- Error/null/empty behavior: not a runtime construct.
- Ownership and lifetime: no runtime ownership.
- Concurrency/global state: none.
- Portability/platform constraints: default portable branch used by ordinary non-Windows builds.
- Performance shape: compile-time only.
- Security or parsing constraints: none.

## Direct Call Sites

Source search found broad `N_CJSON_PUBLIC(...)` use on cJSON declarations in `n_cjson.h` and matching definitions in `n_cjson.c`, plus one focused test fake declaration. This specific conditional branch is selected only by build flags, not by a separate call site.

## Role In Callers

Provides the undecorated public cJSON API surface for default non-Windows builds while preserving the function return type supplied as the macro argument.

## Tests Observed

Default platform compilation and focused tests exercise this branch on ordinary non-Windows builds. There is no focused test specifically dedicated to the macro expansion.

## Graph Links

- Graph node id: `macro:n_cjson.h:144:N_CJSON_PUBLIC`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a compile-time decorator macro.

## Evidence Gaps

No unresolved source gaps for this node.
