# `NOTE_MD5_HASH_SIZE`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:1528`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_MD5_HASH_SIZE`
- Macro aliases/wrappers: literal `16`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, field access, or wrapper expression.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the binary MD5 digest size in bytes.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers can use this value/expression wherever `note.h` is included.
- Exact source search distinguishes macro use from related literal or function use.

## Direct Call Sites

Used by `NoteMD5HashString`, `NoteMD5HashToString`, and related local buffers/loops in `n_md5.c`.

## Role In Callers

Sizes binary digest buffers and hex conversion loops for MD5 helper APIs.

## Tests Observed

Focused binary-store receive tests use MD5 string generation; MD5 helpers are exercised through binary helpers.

## Graph Links

- Graph node id: `macro:note.h:1528:NOTE_MD5_HASH_SIZE`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to MD5 helper functions.

## Evidence Gaps

No unresolved source gaps for this node.
