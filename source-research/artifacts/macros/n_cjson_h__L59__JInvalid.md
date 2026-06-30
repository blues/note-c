# `JInvalid`

## Status

Finalized. Source-inspected cJSON invalid type tag.

## Identity

- Kind: macro
- Definition: `n_cjson.h:59`
- Declaration/interface: public `J.type` tag macro
- Public/exported names: `JInvalid`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JInvalid (0)`
- Visibility: public cJSON type constant
- Ownership/lifetime role: marks an invalid or uninitialized base JSON type

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: compared against the low byte of `J.type`
- Artifact coverage: source-inspected artifact with verified incoming constant-use edges

## Behavior

Defines the invalid base type as zero. Zeroed `J` items begin with this type value until parse/create code assigns a concrete JSON type. Type checks compare `item->type & 0xFF` against this value.

## Important Boundaries

- Preconditions and assumptions: applies to the public `J.type` field; ownership flags occupy bits above the low byte.
- Error/null/empty behavior: `JIsInvalid` returns false for null input and true only when the low byte is `JInvalid`.
- Ownership and lifetime: no direct ownership behavior; `JDelete` handles allocation fields independently of the invalid tag.
- Concurrency/global state: none.
- Portability/platform constraints: integer preprocessor constant.
- Performance shape: constant-time bitmasked comparisons.
- Security or parsing constraints: `JCompare` rejects invalid values.

## Direct Call Sites

Source-inspected uses include `JIsInvalid` and `JCompare`. `_add_item_to_object` also initializes a local replacement type variable to `JInvalid` before choosing ownership semantics.

## Role In Callers

Provides the invalid/default type value for helper classification and comparison rejection.

## Tests Observed

Focused tests set invalid directly in `test/src/JType_test.cpp` and `test/src/JGetType_test.cpp`. Gap: no public creator/parser path intentionally returns an invalid `J`.

## Graph Links

- Graph node id: `macro:n_cjson.h:59:JInvalid`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `JIsInvalid` and `JCompare`.

## Evidence Gaps

No unresolved source gaps for this node.
