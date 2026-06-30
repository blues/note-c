# `JCreateStringReference`

## Status

Finalized. Source-inspected borrowed-pointer JSON string reference constructor and direct caller.

## Identity

- Kind: function
- Definition: `n_cjson.c:2384`
- Declaration/interface: `n_cjson.h:249`
- Public/exported names: `JCreateStringReference`
- Macro aliases/wrappers: defined with `N_CJSON_PUBLIC`
- Signature/API shape: `J *JCreateStringReference(const char *string)`
- Visibility: public cJSON/note-c JSON API
- Ownership/lifetime role: stores borrowed `valuestring` pointer in a reference node

## Dependency Status

- Project-local dependencies:
- `function:n_cjson.c:194:_jNew_Item` finalized
- `function:n_cjson.c:1826:_cast_away_const` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes `item->type` and `item->valuestring`
- Artifact coverage: source, declaration, direct caller, ownership behavior, and focused-test search inspected

## Behavior

Allocates a zeroed `J` with `_jNew_Item`; if allocation succeeds, sets `type = JString | JIsReference` and stores the input pointer in `valuestring` after `_cast_away_const`.

## Important Boundaries

- Preconditions and assumptions: referenced string must outlive the JSON node's use.
- Error/null/empty behavior: allocation failure returns `NULL`; null input can produce a reference string node with null `valuestring`.
- Ownership and lifetime: returned node does not own `valuestring`; `JDelete` skips freeing it because `JIsReference` is set.
- Concurrency/global state: allocation uses SDK allocator hook.
- Portability/platform constraints: uses `_cast_away_const` to store the pointer in mutable `valuestring`.
- Performance shape: O(1).
- Security or parsing constraints: no string validation or copy.

## Direct Call Sites

Exact production caller: `n_helpers.c:1770` `NoteSleep`, avoiding a duplicate of the large sleep payload string before adding it to a `card.attn` command.

## Role In Callers

Lets `NoteSleep` attach a borrowed Base64 payload string without copying it. The request path consumes/deletes the JSON request before `NoteSleep` returns.

## Tests Observed

No direct focused test for this constructor or its deletion behavior was found. `NoteSleep_test.cpp` exercises success/failure paths but does not assert payload reference ownership.

## Graph Links

- Graph node id: `function:n_cjson.c:2384:JCreateStringReference`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
