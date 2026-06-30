# `N_CJSON_NESTING_LIMIT`

## Status

Finalized. Source-inspected cJSON parse nesting limit macro.

## Identity

- Kind: macro
- Definition: `n_cjson.h:176`
- Declaration/interface: default value guarded by `#ifndef N_CJSON_NESTING_LIMIT`
- Public/exported names: `N_CJSON_NESTING_LIMIT`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define N_CJSON_NESTING_LIMIT 100`
- Visibility: public compile-time cJSON parser configuration macro
- Ownership/lifetime role: parser recursion-depth bound, overrideable at build time

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: used by `_parse_array` and `_parse_object` against `parse_buffer.depth`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Provides the default maximum array/object nesting depth for the cJSON parser. `_parse_array` and `_parse_object` reject input when `input_buffer->depth >= N_CJSON_NESTING_LIMIT` before incrementing the depth counter, causing parsing to fail rather than recursing more deeply.

## Important Boundaries

- Preconditions and assumptions: downstream builds may override the macro before including `n_cjson.h`.
- Error/null/empty behavior: not a runtime function; when the limit is reached, parser functions return false and the public parse API reports failure.
- Ownership and lifetime: no allocation or ownership.
- Concurrency/global state: none.
- Portability/platform constraints: compile-time constant, portable across platforms.
- Performance shape: one integer comparison per nested array/object parse entry.
- Security or parsing constraints: bounds parser recursion depth to reduce stack-overflow risk on deeply nested JSON.

## Direct Call Sites

Source search found uses in `_parse_array` at `n_cjson.c:1256` and `_parse_object` at `n_cjson.c:1406`, plus header documentation and the guarded definition. No other production use was found.

## Role In Callers

The parser entry path reaches this macro through the finalized `_parse_array` and `_parse_object` helpers while parsing nested JSON values.

## Tests Observed

No focused test directly exercises the nesting-limit failure boundary. General parser tests cover ordinary nested objects, but not the configured depth limit.

## Graph Links

- Graph node id: `macro:n_cjson.h:176:N_CJSON_NESTING_LIMIT`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `_parse_array` and `_parse_object`.

## Evidence Gaps

No unresolved source gaps for this node.
