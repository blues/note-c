# `JPrintPreallocatedOmitEmpty`

## Status

Finalized. Source-inspected public preallocated JSON print wrapper with omit-empty mode.

## Identity

- Kind: function
- Definition: `n_cjson.c:1118`
- Declaration/interface: `n_cjson.h:203`
- Signature/API shape: `Jbool JPrintPreallocatedOmitEmpty(J *item, char *buf, const int len, const Jbool fmt)`
- Visibility: public through `N_CJSON_PUBLIC`, configured in `n_cjson.h:134-144`
- Public/exported names: `JPrintPreallocatedOmitEmpty`
- Macro aliases/wrappers: none
- Ownership/lifetime role: public preallocated print entry; caller owns buffer

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1097:_printPreallocated` finalized in this slice
- External/system dependencies: none directly
- Data/type/global dependencies: writes through caller buffer via `_printPreallocated`
- Artifact coverage: generated artifact exists

## Behavior

Returns `_printPreallocated(item, buf, len, fmt, true)` directly. This selects the omit-empty object-printing behavior while preserving the caller-provided formatting flag.

## Important Boundaries

- Preconditions and assumptions: caller supplies writable storage; public header warns to over-allocate because estimates are not always exact.
- Error/null/empty behavior: inherits `_printPreallocated` false returns for null item/buffer, negative length, undersized buffer, or print failure.
- Ownership and lifetime: caller retains full responsibility for the buffer; no allocation or free occurs.
- Concurrency/global state: no global state.
- Portability/platform constraints: public export macro is platform/config dependent.
- Performance shape: O(serialized JSON size) with no growth allocation.
- Security or parsing constraints: omit-empty mode skips false, zero, and blank-string object fields through the shared object printer; buffer bounds are enforced by `_ensure`.

## Direct Call Sites

No in-repository production call sites were found beyond the definition and public declaration in `n_cjson.h:203`.

## Role In Callers

This is the public caller-owned-buffer print API for omit-empty output.

## Tests Observed

None. `rg -n "JPrintPreallocated|JPrintPreallocatedOmitEmpty|_printPreallocated" test` found no focused tests.

## Graph Links

- Graph node id: `function:n_cjson.c:1118:JPrintPreallocatedOmitEmpty`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_printPreallocated`.

## Evidence Gaps

No unresolved source gaps for this node. Focused preallocated-print tests were not found.
