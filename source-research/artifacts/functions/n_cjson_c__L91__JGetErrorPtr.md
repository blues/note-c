# `JGetErrorPtr`

## Status

Finalized. Source-inspected public parse-error pointer accessor.

## Identity

- Kind: function
- Definition: `n_cjson.c:91`
- Declaration/interface: `n_cjson.h:217`
- Public/exported names: `JGetErrorPtr`
- Macro aliases/wrappers: none found
- Signature/API shape: `const char *JGetErrorPtr(void)`
- Visibility: public cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: returns a borrowed pointer into the last parsed JSON buffer recorded by parser error state

## Dependency Status

- Project-local dependencies:
- No project-local callees.
- External/system dependencies: none
- Data/type/global dependencies: reads static `global_error.json` and `global_error.position`
- Artifact coverage: source-inspected artifact

## Behavior

Returns `(const char *)(global_error.json + global_error.position)`, where `global_error` is the parser's static error location record.

## Important Boundaries

- Preconditions and assumptions: meaningful only after parse code has set `global_error`; parser success resets it to `{NULL, 0}`.
- Error/null/empty behavior: no guard for `global_error.json == NULL`; default/success state performs pointer arithmetic on a null base before returning.
- Ownership and lifetime: returned pointer is borrowed from the input buffer associated with parser state.
- Concurrency/global state: reads shared static parser error state with no synchronization.
- Portability/platform constraints: no platform-specific behavior beyond pointer arithmetic assumptions.
- Performance shape: O(1).
- Security or parsing constraints: no validation that the stored position is within the original buffer.

## Direct Call Sites

Exact source search found only comments, the public declaration in `n_cjson.h:217`, and the definition in `n_cjson.c:91`. No production repository caller was found.

## Role In Callers

No repository caller role. Public cJSON-compatible API for downstream callers to inspect the last parse error location.

## Tests Observed

None. Focused test search found no direct `JGetErrorPtr` use.

## Graph Links

- Graph node id: `function:n_cjson.c:91:JGetErrorPtr`
- Structured graph: `inventory/dependency-graph.json`
- Verified as a leaf with no project-local callees.

## Evidence Gaps

No unresolved source gaps for this node.
