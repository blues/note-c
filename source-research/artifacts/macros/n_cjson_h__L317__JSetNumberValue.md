# `JSetNumberValue`

## Status

Finalized. Source-inspected public cJSON number value setter macro.

## Identity

- Kind: macro
- Definition: `n_cjson.h:317`
- Declaration/interface: public macro in `n_cjson.h`
- Public/exported names: `JSetNumberValue`
- Macro aliases/wrappers: wrapper around `JSetNumberHelper` for non-null objects
- Signature/API shape: `#define JSetNumberValue(object, number) ((object != NULL) ? JSetNumberHelper(object, (JNUMBER)number) : (number))`
- Visibility: public cJSON value mutation macro
- Ownership/lifetime role: delegates mutation of caller-owned `J` numeric fields

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:322:JSetNumberHelper` finalized
- External/system dependencies: none
- Data/type/global dependencies: target helper writes `J.valueint` and `J.valuenumber`
- Artifact coverage: source-inspected artifact with verified macro-wrapper edge

## Behavior

If `object` is null, returns `number` and does not call the helper. If `object` is non-null, casts `number` to `JNUMBER` and calls `JSetNumberHelper`. The helper saturates `valueint` to `JINTEGER_MAX` or `JINTEGER_MIN` when the numeric value is outside integer bounds, otherwise casts to `JINTEGER`; it then writes `valuenumber` and returns it. The macro does not change `object->type`.

## Important Boundaries

- Preconditions and assumptions: non-null `object` must point to writable `J` storage.
- Error/null/empty behavior: null object is a no-op that returns `number`.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: no global state; mutates caller-provided object.
- Portability/platform constraints: numeric conversion follows the configured `JNUMBER` and `JINTEGER` typedefs.
- Performance shape: one null check plus helper call for non-null objects.
- Security or parsing constraints: integer mirror is saturated, but numeric value itself is stored as provided.

## Direct Call Sites

Exact non-test production search found no invocations; only the macro definition and comments were found.

## Role In Callers

No production repository caller role. Public mutation helper for downstream code that wants cJSON number and integer mirror fields kept consistent with saturation semantics.

## Tests Observed

No exact focused test references to `JSetNumberValue` were found. Gap: no direct macro test for null-object no-op, helper dispatch, saturation, or type preservation.

## Graph Links

- Graph node id: `macro:n_cjson.h:317:JSetNumberValue`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing `expands_to` edge to `JSetNumberHelper`.

## Evidence Gaps

No unresolved source gaps for this node.
