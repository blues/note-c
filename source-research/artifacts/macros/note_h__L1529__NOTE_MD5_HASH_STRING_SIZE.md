# `NOTE_MD5_HASH_STRING_SIZE`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:1529`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_MD5_HASH_STRING_SIZE`
- Macro aliases/wrappers: expression `(((NOTE_MD5_HASH_SIZE)*2)+1)`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: Macro expression dependency on `NOTE_MD5_HASH_SIZE`.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the null-terminated hex MD5 string buffer length derived from the binary digest size.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers can use this value/expression wherever `note.h` is included.
- Exact source search distinguishes macro use from related literal or function use.

## Direct Call Sites

Used in binary store receive/transmit helpers and focused tests for status/hash buffers.

## Role In Callers

Ensures MD5 status strings have room for two hex chars per byte plus terminator.

## Tests Observed

Focused `NoteBinaryStoreReceive_test.cpp` allocates and passes this size into `NoteMD5HashString`.

## Graph Links

- Graph node id: `macro:note.h:1529:NOTE_MD5_HASH_STRING_SIZE`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges to binary helper call sites and `expands_to` dependency on `NOTE_MD5_HASH_SIZE`.

## Evidence Gaps

No unresolved source gaps for this node.
