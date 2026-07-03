# `JConvertToJSONString`

## Status

Finalized. Source-inspected public cJSON print alias macro.

## Identity

- Kind: macro
- Definition: `n_cjson.h:310`
- Declaration/interface: public macro in `n_cjson.h`
- Public/exported names: `JConvertToJSONString`
- Macro aliases/wrappers: aliases `JPrintUnformatted`
- Signature/API shape: `#define JConvertToJSONString JPrintUnformatted`
- Visibility: public cJSON compatibility/helper macro
- Ownership/lifetime role: forwards JSON-string allocation to `JPrintUnformatted`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1051:JPrintUnformatted` finalized
- External/system dependencies: none
- Data/type/global dependencies: inherits target print helper use of configured allocation/free hooks
- Artifact coverage: source-inspected artifact with verified macro-alias edge

## Behavior

Expands `JConvertToJSONString(...)` to `JPrintUnformatted(...)`. The target function returns `NULL` for a null item; otherwise it allocates and returns a compact JSON string by calling the internal print routine with formatting disabled.

## Important Boundaries

- Preconditions and assumptions: non-null argument must point to a valid `J` tree accepted by the printer.
- Error/null/empty behavior: null item or allocation/print failure returns `NULL` through the target function.
- Ownership and lifetime: successful return is a heap allocation owned by the caller and freed through the configured cJSON free path.
- Concurrency/global state: depends on the target's allocation hooks; no macro-local state.
- Portability/platform constraints: preprocessor alias over portable cJSON print API.
- Performance shape: same as unformatted JSON printing.
- Security or parsing constraints: does not validate beyond the target printer's tree traversal.

## Direct Call Sites

Exact non-test production search found only the macro definition. No repository production caller of `JConvertToJSONString` was found.

## Role In Callers

No production repository caller role. Public compatibility alias for downstream code that names conversion to a JSON string.

## Tests Observed

No exact focused test references to `JConvertToJSONString` were found. Existing `JPrintUnformatted`/JSON-printing tests provide target-function coverage, not macro-name coverage.

## Graph Links

- Graph node id: `macro:n_cjson.h:310:JConvertToJSONString`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing `expands_to` edge to `JPrintUnformatted`.

## Evidence Gaps

No unresolved source gaps for this node.
