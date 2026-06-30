# `_printPreallocated`

## Status

Finalized. Source-inspected JSON preallocated-buffer print helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1097`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `Jbool _printPreallocated(J *item, char *buf, const int len, const Jbool fmt, const Jbool omit)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none
- Macro aliases/wrappers: public wrappers `JPrintPreallocated` and `JPrintPreallocatedOmitEmpty`
- Ownership/lifetime role: wraps caller-owned print storage without taking ownership

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1179:_print_value` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: initializes stack `printbuffer` over caller-provided storage
- Artifact coverage: generated artifact exists

## Behavior

Returns `false` when `item` is `NULL`, `buf` is `NULL`, or `len < 0`. Otherwise builds a stack `printbuffer` with `buffer=buf`, `length=(size_t)len`, `offset=0`, `noalloc=true`, `format=fmt`, and `omitempty=omit`, then returns `_print_value(item, &p)` directly.

Because the printbuffer has `noalloc=true`, downstream `_ensure` calls never grow the caller buffer. `len == 0` is accepted by this helper but downstream printing fails once `_ensure` cannot fit requested output plus terminator.

## Important Boundaries

- Preconditions and assumptions: caller supplies writable storage large enough for the selected formatted/unformatted JSON plus terminator.
- Error/null/empty behavior: null item/buffer and negative length fail before printing; undersized buffers fail through print callees. Partial bytes may already have been written before a later recursive failure.
- Ownership and lifetime: caller retains full responsibility for `buf`; this helper does not allocate, free, or return a length.
- Concurrency/global state: no global state.
- Portability/platform constraints: public header warns that size estimation is not exact and callers should over-allocate by 5 bytes.
- Performance shape: O(serialized JSON size) with no realloc growth path.
- Security or parsing constraints: bounded by `_ensure` capacity checks, including `INT_MAX` rejection in the shared print path.

## Direct Call Sites

Exact source search found direct callers:

- `n_cjson.c:1120` `JPrintPreallocatedOmitEmpty` passes `omit=true`.
- `n_cjson.c:1125` `JPrintPreallocated` passes `omit=false`.

## Role In Callers

This helper is the common implementation for public preallocated print APIs. It binds caller memory and print options into the shared `_print_value` serializer.

## Tests Observed

None. `rg -n "JPrintPreallocated|JPrintPreallocatedOmitEmpty|_printPreallocated" test` found no focused tests. Closest print tests exercise `JPrintUnformatted`, which goes through `_print` rather than this helper.

## Graph Links

- Graph node id: `function:n_cjson.c:1097:_printPreallocated`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_print_value`.

## Evidence Gaps

No unresolved source gaps for this node. Focused preallocated-print tests were not found.
