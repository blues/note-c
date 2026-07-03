# `JConvertFromJSONString`

## Status

Finalized. Source-inspected public cJSON parse alias macro.

## Identity

- Kind: macro
- Definition: `n_cjson.h:311`
- Declaration/interface: public macro in `n_cjson.h`
- Public/exported names: `JConvertFromJSONString`
- Macro aliases/wrappers: aliases `JParse`
- Signature/API shape: `#define JConvertFromJSONString JParse`
- Visibility: public cJSON compatibility/helper macro
- Ownership/lifetime role: forwards JSON parsing/allocation to `JParse`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:976:JParse` finalized
- External/system dependencies: none
- Data/type/global dependencies: inherits target parse helper use of `global_error` and configured allocation/free hooks
- Artifact coverage: source-inspected artifact with verified macro-alias edge

## Behavior

Expands `JConvertFromJSONString(...)` to `JParse(...)`. The target function calls `JParseWithOpts(value, 0, 0)`, returns `NULL` for null input or parse failure, and can update the global parse-error pointer on non-null failures.

## Important Boundaries

- Preconditions and assumptions: argument should be a readable NUL-terminated JSON string.
- Error/null/empty behavior: null or invalid JSON returns `NULL` through `JParse`.
- Ownership and lifetime: successful return is a `J *` tree owned by the caller and freed with `JDelete`.
- Concurrency/global state: parse failure behavior can update cJSON global error state through the target parse path.
- Portability/platform constraints: preprocessor alias over portable cJSON parse API.
- Performance shape: same as `JParse`.
- Security or parsing constraints: same parser depth, allocation, and JSON syntax limits as `JParseWithOpts`.

## Direct Call Sites

Exact non-test production search found only the macro definition. No repository production caller of `JConvertFromJSONString` was found.

## Role In Callers

No production repository caller role. Public compatibility alias for downstream code that names conversion from a JSON string.

## Tests Observed

No exact focused test references to `JConvertFromJSONString` were found. Existing `JParse` tests provide target-function coverage, not macro-name coverage.

## Graph Links

- Graph node id: `macro:n_cjson.h:311:JConvertFromJSONString`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing `expands_to` edge to `JParse`.

## Evidence Gaps

No unresolved source gaps for this node.
