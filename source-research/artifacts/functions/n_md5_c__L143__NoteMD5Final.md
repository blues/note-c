# `NoteMD5Final`

## Status

Finalized. Source-inspected MD5 finalization routine.

## Identity

- Kind: function
- Definition: `n_md5.c:143`
- Declaration/interface: `note.h:1551`
- Signature/API shape: `void NoteMD5Final(unsigned char *digest, NoteMD5Context *ctx)`
- Visibility: public header declaration, implemented in `n_md5.c`
- Public/exported names: `NoteMD5Final`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_md5.c:201:NoteMD5Transform` finalized
  - `function:n_md5.c:48:putu32` finalized
- External/system dependencies: `memset`
- Data/type/global dependencies: mutates caller-provided `digest` and `ctx`; no global state
- Artifact coverage: implementation, declaration, callees, callers, and direct searches inspected

## Behavior

Finalizes an MD5 context by computing the current byte count modulo 64, appending `0x80`, zero-padding to the MD5 length field position, and transforming one or two final blocks as needed. It writes the low and high bit counts into `ctx->in + 56` and `ctx->in + 60`, transforms the final block, serializes the four state words into the 16-byte caller digest with `putu32`, and clears the context with `memset(ctx, 0, sizeof(NoteMD5Context))`.

## Important Boundaries

- Preconditions and assumptions: caller must pass a writable 16-byte digest buffer and a valid initialized `NoteMD5Context`; no null or bounds checks.
- Error/null/empty behavior: none; invalid pointers are undefined C behavior.
- Ownership and lifetime: writes caller-provided digest, mutates and then clears caller-provided context, allocates nothing.
- Concurrency/global state: no global state; all state is in caller-provided context.
- Portability/platform constraints: uses `putu32` for little-endian serialization.
- Performance shape: O(1), with one or two fixed block transforms.
- Security/parsing constraints: context is cleared after digest output, reducing residual state lifetime.

## Direct Call Sites

Exact source search `rg -n "\bNoteMD5Final\b" n_md5.c note.h` found:

- `note.h:1551` declaration.
- `n_md5.c:15` source comment describing normal use.
- `n_md5.c:143` definition.
- `n_md5.c:297` called by `NoteMD5Hash`.

## Role In Callers

`NoteMD5Hash` uses `NoteMD5Final` after `NoteMD5Init` and `NoteMD5Update` to produce a 16-byte digest in the caller-provided output buffer.

## Tests Observed

None. No direct focused production-symbol test was inspected for this finalization routine.

## Graph Links

- Graph node id: `function:n_md5.c:143:NoteMD5Final`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteMD5Transform` and `putu32`.
- Verified incoming edge from `NoteMD5Hash`.

## Evidence Gaps

No unresolved source gaps for this node.
