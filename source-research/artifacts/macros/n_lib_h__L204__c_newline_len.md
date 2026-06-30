# `c_newline_len`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:204`
- Declaration/interface: shared internal string-length macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `2`, paired with `c_newline = "\r\n"`.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: translation units including `n_lib.h`.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Provides the compile-time byte length for the paired global constant string declared in `n_lib.h` and defined in `n_const.c`. Used by `_serialNoteTransaction` when appending CRLF after serial request transmission.

## Important Boundaries

- Compile-time literal; no storage or ownership.
- Must stay synchronized with the paired `c_*` string definition in `n_const.c`.
- Exact search separated `_len` use from paired string use.

## Direct Call Sites

Used by `_serialNoteTransaction` when appending CRLF after serial request transmission.

## Role In Callers

Avoids repeated runtime length computation for shared strings where callers need exact token lengths.

## Tests Observed

Focused serial transaction tests assert CRLF transmit behavior.

## Graph Links

- Graph node id: `macro:n_lib.h:204:c_newline_len`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edge to `_serialNoteTransaction`.

## Evidence Gaps

No unresolved source gaps for this node.
