# `NoteMD5Hash`

## Status

Finalized. Source-inspected one-shot MD5 digest wrapper.

## Identity

- Kind: function
- Definition: `n_md5.c:292`
- Declaration/interface: `note.h:1566`
- Signature/API shape: `void NoteMD5Hash(unsigned char* data, unsigned long len, unsigned char *retHash)`
- Visibility: public header declaration, implemented in `n_md5.c`
- Public/exported names: `NoteMD5Hash`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_md5.c:78:NoteMD5Init` finalized
  - `function:n_md5.c:95:NoteMD5Update` finalized
  - `function:n_md5.c:143:NoteMD5Final` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: stack `NoteMD5Context`; writes caller-provided digest buffer
- Artifact coverage: implementation, declaration, callee chain, caller search, and graph node inspected

## Behavior

Allocates a `NoteMD5Context` on the stack, initializes it, updates it once with the full caller-provided input buffer and length, then finalizes into the caller-provided 16-byte digest buffer.

## Important Boundaries

- Preconditions and assumptions: caller must pass a writable digest buffer and, when `len > 0`, a readable data buffer; no null or bounds checks.
- Error/null/empty behavior: no explicit error path.
- Ownership and lifetime: borrows input data, writes caller digest, uses only stack context, allocates nothing.
- Concurrency/global state: no global state.
- Portability/platform constraints: inherits the portable byte-order and 32-bit-masked behavior of the lower MD5 routines.
- Performance shape: O(len).

## Direct Call Sites

Exact source search `rg -n "\bNoteMD5Hash\b|\bNoteMD5HashString\b|\bNoteMD5HashToString\b" -g '!source-research/**' -g '!docs/**'` found:

- `note.h:1566` declaration.
- `n_md5.c:292` definition.
- `n_md5.c:304` called by `NoteMD5HashString`.

No production source outside `n_md5.c` calls `NoteMD5Hash` directly in this tree.

## Role In Callers

`NoteMD5HashString` uses `NoteMD5Hash` to produce a 16-byte binary digest before converting each byte to hex.

## Tests Observed

None. No focused direct `NoteMD5Hash` production-symbol test was inspected.

## Graph Links

- Graph node id: `function:n_md5.c:292:NoteMD5Hash`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteMD5Init`, `NoteMD5Update`, and `NoteMD5Final`.
- Verified incoming edge from `NoteMD5HashString`.

## Evidence Gaps

No unresolved source gaps for this node.
