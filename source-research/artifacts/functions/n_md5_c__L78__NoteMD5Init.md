# `NoteMD5Init`

## Status

Finalized. Source-inspected MD5 context initializer.

## Identity

- Kind: function
- Definition: `n_md5.c:78`
- Declaration/interface: `note.h:1536`
- Signature/API shape: `void NoteMD5Init(NoteMD5Context *ctx)`
- Visibility: public header declaration, implemented in `n_md5.c`
- Public/exported names: `NoteMD5Init`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `memset`
- Data/type/global dependencies: mutates caller-provided `NoteMD5Context`; no global state
- Artifact coverage: implementation, declaration, caller search, and graph node inspected

## Behavior

Zeros the entire caller-provided `NoteMD5Context`, then initializes the four MD5 state words to the standard constants `0x67452301`, `0xefcdab89`, `0x98badcfe`, and `0x10325476`, and initializes both bit counters to zero.

## Important Boundaries

- Preconditions and assumptions: caller must pass a writable `NoteMD5Context *`; no null check.
- Error/null/empty behavior: none; invalid pointer is undefined C behavior.
- Ownership and lifetime: initializes caller-owned context and allocates nothing.
- Concurrency/global state: no global state.
- Portability/platform constraints: uses unsigned long state words per this implementation's portability strategy.
- Performance shape: O(1).

## Direct Call Sites

Exact source search `rg -n "\bNoteMD5Init\b" n_md5.c note.h` found:

- `note.h:1536` declaration.
- `n_md5.c:15` source comment describing normal use.
- `n_md5.c:78` definition.
- `n_md5.c:295` called by `NoteMD5Hash`.

## Role In Callers

`NoteMD5Hash` calls `NoteMD5Init` on a stack `NoteMD5Context` before updating it with input data and finalizing the digest.

## Tests Observed

None. No direct focused production-symbol test was inspected for this initializer.

## Graph Links

- Graph node id: `function:n_md5.c:78:NoteMD5Init`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming edge from `NoteMD5Hash`.

## Evidence Gaps

No unresolved source gaps for this node.
