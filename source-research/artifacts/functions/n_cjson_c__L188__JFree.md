# `JFree`

## Status

Finalized. Source-inspected public CJSON free wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:188`
- Declaration/interface: `n_cjson.h:326`
- Public/exported names: `JFree`
- Macro aliases/wrappers: none for `JFree`; implementation delegates through internal `_Free` alias in `n_lib.h:242`
- Signature/API shape: `void JFree(void *p)`
- Visibility: public cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: releases caller-provided memory through the SDK free hook

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:242:_Free` expands to finalized `NoteFree`
- External/system dependencies: free hook used by `NoteFree`
- Data/type/global dependencies: reads free-hook state through `NoteFree`
- Artifact coverage: source-inspected artifact

## Behavior

Calls `_Free(p)`, where `_Free` is the internal alias for `NoteFree`, and returns. If no free hook is registered, the delegated call is a no-op; otherwise the pointer is passed to the hook unchanged.

## Important Boundaries

- Preconditions and assumptions: pointer must be appropriate for the currently configured free hook; the source does not track allocator provenance.
- Error/null/empty behavior: returns no status; null pointers are forwarded to `NoteFree` and then to the hook if one is configured.
- Ownership and lifetime: releases memory according to the registered platform hook's contract.
- Concurrency/global state: delegates to `NoteFree`, which reads hook globals without taking the Notecard lock.
- Portability/platform constraints: deallocation policy stays behind SDK hooks rather than direct libc calls in this wrapper.
- Performance shape: one free-wrapper call plus optional debug work in `NoteFree`.
- Security or parsing constraints: changing allocation/free hooks between allocation and `JFree` can mismatch memory ownership conventions.

## Direct Call Sites

Exact production source search found only the public declaration in `n_cjson.h:326` and the definition in `n_cjson.c:188`. No production repository caller was found.

## Role In Callers

No production repository caller role. Tests and public comments use `JFree` as the release API for buffers returned by JSON print paths and helper APIs.

## Tests Observed

Focused cleanup usage appears in `test/src/JPrintUnformatted_test.cpp:55` and `test/src/JSON_number_handling_test.cpp` for printed JSON output. Underlying hook behavior is covered by `test/src/NoteFree_test.cpp`, which verifies unset hooks no-op and set hooks receive the pointer. No direct test assertion covers `JFree(NULL)`.

## Graph Links

- Graph node id: `function:n_cjson.c:188:JFree`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_Free` / `NoteFree`.

## Evidence Gaps

No unresolved source gaps for this node.
