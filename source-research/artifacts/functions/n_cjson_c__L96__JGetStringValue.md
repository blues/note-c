# `JGetStringValue`

## Status

Finalized. Source-inspected public string value accessor.

## Identity

- Kind: function
- Definition: `n_cjson.c:96`
- Declaration/interface: `n_cjson.h:220`
- Public/exported names: `JGetStringValue`
- Macro aliases/wrappers: none found
- Signature/API shape: `char *JGetStringValue(J *item)`
- Visibility: public cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: returns a borrowed pointer from the caller-owned item

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2713:JIsString` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads `item->valuestring`
- Artifact coverage: source-inspected artifact

## Behavior

Returns `NULL` when `JIsString(item)` is false. Otherwise returns `item->valuestring`.

## Important Boundaries

- Preconditions and assumptions: caller may pass null or non-string items; the type predicate handles that check.
- Error/null/empty behavior: null item, non-string item, or string item with null `valuestring` returns `NULL`.
- Ownership and lifetime: returned pointer is borrowed from the JSON item; no allocation or ownership transfer.
- Concurrency/global state: no global state.
- Portability/platform constraints: no platform-specific behavior.
- Performance shape: O(1).
- Security or parsing constraints: does not copy or validate string storage.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:220` and the definition in `n_cjson.c:96`. No production repository caller was found.

## Role In Callers

No repository caller role. Public cJSON-compatible accessor for downstream callers.

## Tests Observed

None. Focused test search found no direct `JGetStringValue` use.

## Graph Links

- Graph node id: `function:n_cjson.c:96:JGetStringValue`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `JIsString`.

## Evidence Gaps

No unresolved source gaps for this node.
