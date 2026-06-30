# `JSetIntValue`

## Status

Finalized. Source-inspected public cJSON integer value setter macro.

## Identity

- Kind: macro
- Definition: `n_cjson.h:314`
- Declaration/interface: public macro in `n_cjson.h`
- Public/exported names: `JSetIntValue`
- Macro aliases/wrappers: none found
- Signature/API shape: `#define JSetIntValue(object, number) ((object) ? (object)->valueint = (object)->valuenumber = (number) : (number))`
- Visibility: public cJSON value mutation macro
- Ownership/lifetime role: mutates caller-owned `J` object numeric fields

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes `J.valueint` and `J.valuenumber` when `object` is non-null
- Artifact coverage: source-inspected artifact

## Behavior

If `object` is non-null, assigns `number` to `object->valuenumber`, assigns that result to `object->valueint`, and returns the assigned value. If `object` is null, returns `number` and performs no write. The macro does not change `object->type`.

## Important Boundaries

- Preconditions and assumptions: non-null `object` must point to writable `J` storage; `number` must be suitable for assignment to both numeric fields.
- Error/null/empty behavior: null object is a no-op that returns `number`.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: no global state; mutates caller-provided object.
- Portability/platform constraints: macro assignment expression; type conversion follows C assignment rules for the fields.
- Performance shape: one null test and up to two field writes.
- Security or parsing constraints: no saturation, validation, or type update.

## Direct Call Sites

Exact non-test production search found only the macro definition and nearby comments. No repository production caller of `JSetIntValue` was found.

## Role In Callers

No production repository caller role. Public mutation helper for downstream code that wants to keep integer and numeric cJSON fields synchronized.

## Tests Observed

No exact focused test references to `JSetIntValue` were found. Gap: no direct test for null-object no-op, field synchronization, or conversion behavior.

## Graph Links

- Graph node id: `macro:n_cjson.h:314:JSetIntValue`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this macro performs direct field writes rather than calling a project-local function.

## Evidence Gaps

No unresolved source gaps for this node.
