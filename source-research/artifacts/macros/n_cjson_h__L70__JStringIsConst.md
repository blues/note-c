# `JStringIsConst`

## Status

Finalized. Source-inspected cJSON constant object-key ownership flag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:70`
- Declaration/interface: public `J.type` ownership flag macro
- Public/exported names: `JStringIsConst`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JStringIsConst 512`
- Visibility: public cJSON type/ownership flag
- Ownership/lifetime role: marks an object key string as constant/referenced rather than owned

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: ORed into or cleared from `J.type`; checked for `J.string` ownership
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines an ownership flag above the low type byte for object key strings. Constant-string object insertion sets it; normal object insertion and replacement clear it. `JDelete` skips freeing `item->string` when it is set, and `JDuplicate` preserves the const key pointer rather than duplicating it.

## Important Boundaries

- Preconditions and assumptions: base type remains in `item->type & 0xFF`; this flag only changes key-string ownership behavior.
- Error/null/empty behavior: not a runtime function; delete/duplicate paths handle null pointers independently.
- Ownership and lifetime: when set, caller/static storage must keep the key string valid for the node lifetime.
- Concurrency/global state: none.
- Portability/platform constraints: integer flag outside the low type byte.
- Performance shape: constant-time bit tests/updates.
- Security or parsing constraints: misuse can create dangling key pointers or leaks.

## Direct Call Sites

Source-inspected uses include `JDelete`, `_add_item_to_object`, `_replace_item_in_object`, and `JDuplicate`.

## Role In Callers

Controls ownership of object key strings across add, replace, delete, and duplicate operations.

## Tests Observed

No obvious focused test directly asserts constant-key ownership behavior.

## Graph Links

- Graph node id: `macro:n_cjson.h:70:JStringIsConst`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from object add/replace helpers, `JDelete`, and `JDuplicate`.

## Evidence Gaps

No unresolved source gaps for this node.
