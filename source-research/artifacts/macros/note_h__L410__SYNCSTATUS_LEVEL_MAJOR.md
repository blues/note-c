# `SYNCSTATUS_LEVEL_MAJOR`

## Status

Finalized. Source-inspected definition, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `note.h:410`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `SYNCSTATUS_LEVEL_MAJOR`
- Macro aliases/wrappers: literal `0`.
- Signature/API shape: preprocessor literal or expression macro
- Visibility: public to translation units including `note.h`.
- Ownership/lifetime role: no ownership; compile-time substitution only.

## Dependency Status

- Project-local dependencies: No project-local callees; macro expands to a literal, field access, or wrapper expression.
- External/system dependencies: none.
- Data/type/global dependencies: caller-owned inputs and direct consumers listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Defines the public maximum level value for `NoteDebugSyncStatus` callers requesting major sync status only. The implementation prints a queued sync note when `maxLevel < 0 || JGetInt(body, "level") <= maxLevel`.

## Important Boundaries

- Compile-time macro; no runtime storage or ownership.
- Public header consumers can use this value/expression wherever `note.h` is included.
- Exact source search distinguishes macro use from related literal or function use.

## Direct Call Sites

Exact source search found no production macro uses outside the public definitions; `NoteDebugSyncStatus` accepts the numeric max-level contract at `n_helpers.c:2332`.

## Role In Callers

Communicates the intended `maxLevel` values for sync-status filtering without exposing magic numbers to SDK callers.

## Tests Observed

Focused `NoteDebugSyncStatus_test.cpp` covers max-level filtering behavior, including the all-level negative path; tests mostly pass literals rather than these macros.

## Graph Links

- Graph node id: `macro:note.h:410:SYNCSTATUS_LEVEL_MAJOR`
- Structured graph: `inventory/dependency-graph.json`
- Semantics verified against `NoteDebugSyncStatus`.

## Evidence Gaps

No unresolved source gaps for this node.
