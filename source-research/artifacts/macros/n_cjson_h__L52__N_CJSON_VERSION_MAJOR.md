# `N_CJSON_VERSION_MAJOR`

## Status

Finalized. Source-inspected public cJSON major-version macro.

## Identity

- Kind: macro
- Definition: `n_cjson.h:52`
- Declaration/interface: public/internal cJSON compatibility header `n_cjson.h`
- Public/exported names: `N_CJSON_VERSION_MAJOR`
- Macro aliases/wrappers: stringized by `NOTE_C_STRINGIZE` in `JVersion`
- Signature/API shape: object-like macro `N_CJSON_VERSION_MAJOR 1`
- Visibility: any translation unit including `n_cjson.h`
- Ownership/lifetime role: compile-time version component

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: used by `n_cjson.c` compile-time header/source version guard and `JVersion`
- Artifact coverage: source-inspected artifact

## Behavior

Defines the cJSON major version as `1`. `n_cjson.c` checks this macro against its expected value and includes it in the string returned by `JVersion`.

## Important Boundaries

- Preconditions and assumptions: must stay synchronized with the implementation's version guard.
- Error/null/empty behavior: mismatch with implementation expected value triggers a compile-time `#error`.
- Ownership and lifetime: preprocessor-only.
- Concurrency/global state: none.
- Portability/platform constraints: header/source consistency guard.
- Performance shape: no runtime cost except inclusion in `JVersion` string literal construction.
- Security or parsing constraints: none.

## Direct Call Sites

Source inspection found uses in `n_cjson.c` version compatibility guard and `JVersion`.

## Role In Callers

Public version component exposed through `n_cjson.h` and `JVersion`.

## Tests Observed

Focused `JVersion` tests cover the resulting version string; no test targets this macro directly.

## Graph Links

- Graph node id: `macro:n_cjson.h:52:N_CJSON_VERSION_MAJOR`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none.

## Evidence Gaps

No unresolved source gaps for this node.
