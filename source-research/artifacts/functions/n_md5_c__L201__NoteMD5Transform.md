# `NoteMD5Transform`

## Status

Finalized. Source-inspected MD5 64-byte block transform.

## Identity

- Kind: function
- Definition: `n_md5.c:201`
- Declaration/interface: `note.h:1558`
- Signature/API shape: `void NoteMD5Transform(unsigned long buf[4], const unsigned char inraw[64])`
- Visibility: public header declaration, implemented in `n_md5.c`
- Public/exported names: `NoteMD5Transform`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_md5.c:42:getu32` finalized
  - `macro:n_md5.c:193:MD5STEP` finalized
  - `macro:n_md5.c:187:F1`, `macro:n_md5.c:188:F2`, `macro:n_md5.c:189:F3`, `macro:n_md5.c:190:F4` finalized through source-inspected MD5STEP argument use
- External/system dependencies: primitive integer arithmetic, shifts, bitwise operations
- Data/type/global dependencies: mutates caller-provided `buf[4]`; reads caller-provided 64-byte `inraw`
- Artifact coverage: implementation, declaration, callees, callers, and graph gaps inspected

## Behavior

Converts the 64-byte input block into sixteen little-endian words with `getu32`, copies the four current state words into local `a`, `b`, `c`, and `d`, then applies 64 `MD5STEP` operations grouped into four 16-step rounds using `F1`, `F2`, `F3`, and `F4`. After the rounds, it adds the local state back into `buf[0..3]`.

## Important Boundaries

- Preconditions and assumptions: caller must provide a writable four-word state buffer and a readable 64-byte input block; no null or bounds checks.
- Error/null/empty behavior: none; invalid pointers are undefined C behavior.
- Ownership and lifetime: borrows `inraw`, mutates `buf`, and allocates nothing.
- Concurrency/global state: no static mutable or global state.
- Portability/platform constraints: input byte order is normalized through `getu32`; `MD5STEP` masks working words to 32 bits to tolerate wider `unsigned long`.
- Performance shape: fixed O(1) transform per 64-byte block.

## Direct Call Sites

Exact source search `rg -n "\bNoteMD5Transform\b" n_md5.c note.h` found:

- `note.h:1558` declaration.
- `n_md5.c:120` called by `NoteMD5Update` for a completed buffered block.
- `n_md5.c:129` called by `NoteMD5Update` for each full 64-byte input block.
- `n_md5.c:163` called by `NoteMD5Final` when padding spans two blocks.
- `n_md5.c:176` called by `NoteMD5Final` for the final padded block.
- `n_md5.c:201` definition.

## Role In Callers

`NoteMD5Update` uses this transform to absorb one or more complete 64-byte message blocks. `NoteMD5Final` uses it to process the padded final block(s) and then serializes the final digest.

## Tests Observed

None. This transform is covered only through higher-level MD5 use in the current source-research slice.

## Graph Links

- Graph node id: `function:n_md5.c:201:NoteMD5Transform`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming edges from `NoteMD5Update` and `NoteMD5Final`.
- Verified outgoing edges to `getu32`, `MD5STEP`, and source-repaired macro-use edges to `F1`-`F4`.

## Evidence Gaps

No unresolved source gaps for this node.
