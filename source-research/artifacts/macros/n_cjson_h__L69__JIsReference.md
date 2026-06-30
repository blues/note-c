# `JIsReference`

## Status

Finalized. Source-inspected cJSON reference ownership flag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:69`
- Declaration/interface: public `J.type` ownership flag macro
- Public/exported names: `JIsReference`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JIsReference 256`
- Visibility: public cJSON type/ownership flag
- Ownership/lifetime role: marks child/value storage as referenced rather than owned

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: ORed into `J.type`; checked by `JDelete` and cleared by `JDuplicate`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines an ownership flag above the low type byte. Reference constructors and `_create_reference` OR it into the base type. `JDelete` skips freeing `child` and `valuestring` when it is set, while `JDuplicate` clears the flag on duplicated items.

## Important Boundaries

- Preconditions and assumptions: base type remains in `item->type & 0xFF`; this flag only changes ownership behavior.
- Error/null/empty behavior: not a runtime function; delete/duplicate paths handle nulls independently.
- Ownership and lifetime: prevents `JDelete` from freeing referenced child/value storage; caller must ensure referenced storage outlives the `J` node.
- Concurrency/global state: none.
- Portability/platform constraints: integer flag outside the low type byte.
- Performance shape: constant-time bit tests/updates.
- Security or parsing constraints: misuse can create dangling references or leaks.

## Direct Call Sites

Source-inspected uses include `JDelete`, `_create_reference`, `JCreateStringReference`, `JCreateObjectReference`, `JCreateArrayReference`, and `JDuplicate`. Production code outside cJSON uses `JCreateStringReference` in the sleep-payload helper path.

## Role In Callers

Separates owned cJSON nodes from reference nodes so delete and duplicate behavior can preserve caller-owned storage.

## Tests Observed

No obvious focused unit test directly asserts reference delete/duplicate ownership. Reference constructor paths have indirect coverage through callers that use reference APIs.

## Graph Links

- Graph node id: `macro:n_cjson.h:69:JIsReference`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from reference constructors, `JDelete`, `_create_reference`, and `JDuplicate`.

## Evidence Gaps

No unresolved source gaps for this node.
