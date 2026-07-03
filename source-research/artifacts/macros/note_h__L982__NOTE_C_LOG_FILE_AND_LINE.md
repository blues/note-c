# `NOTE_C_LOG_FILE_AND_LINE`

## Status

Finalized. Source-inspected declaration, direct references, ownership role, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:982`
- Declaration/interface: source/header type declaration as noted.
- Public/exported names: `NOTE_C_LOG_FILE_AND_LINE` where named; generated placeholder names are internal extractor labels.
- Macro aliases/wrappers: not applicable.
- Signature/API shape: debug logging macro with `lvl` parameter
- Visibility: determined by the declaring source/header.
- Ownership/lifetime role: described below.

## Dependency Status

- Project-local dependencies: Depends on `NOTE_C_STRINGIZE` and `NoteDebugWithLevel` in the enabled branch.
- External/system dependencies: standard C types and SDK typedefs referenced in the declaration.
- Data/type/global dependencies: direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

When `NOTE_C_LOG_SHOW_FILE_AND_LINE` is enabled, emits `__FILE__`, a stringized `__LINE__`, and a space through `NoteDebugWithLevel`; otherwise expands to an empty macro.

## Important Boundaries

- Compile-time type contract; no runtime code by itself.
- Ownership and lifetime are enforced by the functions that allocate, store, or consume values of this type.
- Direct source references were inspected with exact symbol search.

## Direct Call Sites

Used by `NOTE_C_LOG_ERROR`, `NOTE_C_LOG_WARN`, `NOTE_C_LOG_INFO`, and `NOTE_C_LOG_DEBUG` wrappers.

## Role In Callers

Adds optional source-location prefixing to log output without changing log-level filtering.

## Tests Observed

Focused debug tests cover log-level filtering but do not enable file/line prefixing.

## Graph Links

- Graph node id: `macro:note.h:982:NOTE_C_LOG_FILE_AND_LINE`
- Structured graph: `inventory/dependency-graph.json`
- Verified edges to `NOTE_C_STRINGIZE`, `NoteDebugWithLevel`, and log wrapper macro users.

## Evidence Gaps

No unresolved source gaps for this node.
