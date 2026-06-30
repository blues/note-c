# `NoteMD5Update`

## Status

Finalized. Source-inspected MD5 streaming update routine.

## Identity

- Kind: function
- Definition: `n_md5.c:95`
- Declaration/interface: `note.h:1544`
- Signature/API shape: `void NoteMD5Update(NoteMD5Context *ctx, unsigned char const *buf, unsigned long len)`
- Visibility: public header declaration, implemented in `n_md5.c`
- Public/exported names: `NoteMD5Update`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_md5.c:201:NoteMD5Transform` finalized
- External/system dependencies: `memcpy`
- Data/type/global dependencies: mutates caller-provided `NoteMD5Context`; reads caller-provided input buffer; no global state
- Artifact coverage: implementation, declaration, callee, caller search, and graph node inspected

## Behavior

Adds `len * 8` bits to the context bit counters, carrying from `bits[0]` to `bits[1]` and adding high bits from `len >> 29`. If a partial block already exists in `ctx->in`, it fills that block and transforms it when complete. It then transforms every full 64-byte block from the input, and finally copies any trailing bytes into `ctx->in` for a later update or finalization.

## Important Boundaries

- Preconditions and assumptions: caller must pass a valid initialized context and, when `len > 0`, a readable buffer of at least `len` bytes; no null or bounds checks.
- Error/null/empty behavior: no explicit error path; `len == 0` falls through to a zero-length trailing `memcpy`.
- Ownership and lifetime: borrows input, mutates caller-owned context, allocates nothing.
- Concurrency/global state: no global state.
- Portability/platform constraints: maintains 64-bit bit count across two 32-bit words and delegates byte-order handling to `NoteMD5Transform`.
- Performance shape: O(len) with fixed 64-byte block processing.

## Direct Call Sites

Exact source search `rg -n "\bNoteMD5Update\b" n_md5.c note.h` found:

- `note.h:1544` declaration.
- `n_md5.c:15` source comment describing normal use.
- `n_md5.c:95` definition.
- `n_md5.c:198` source comment describing transform relationship.
- `n_md5.c:296` called by `NoteMD5Hash`.

## Role In Callers

`NoteMD5Hash` calls `NoteMD5Update` once with the full input buffer after initializing a stack context and before finalizing into the output digest.

## Tests Observed

None. No direct focused production-symbol test was inspected for this update routine.

## Graph Links

- Graph node id: `function:n_md5.c:95:NoteMD5Update`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `NoteMD5Transform`.
- Verified incoming edge from `NoteMD5Hash`.

## Evidence Gaps

No unresolved source gaps for this node.
