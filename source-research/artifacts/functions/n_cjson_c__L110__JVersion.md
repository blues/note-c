# `JVersion`

## Status

Finalized. Source-inspected public CJSON version helper, declaration, macro dependency, and direct source references.

## Identity

- Kind: function
- Definition: `n_cjson.c:110`
- Declaration/interface: `n_cjson.h:180`
- Public/exported names: `JVersion`
- Macro aliases/wrappers: declared/defined through `N_CJSON_PUBLIC`
- Signature/API shape: `const char *JVersion(void)`
- Visibility: public CJSON API exported according to `N_CJSON_PUBLIC`
- Ownership/lifetime role: returns a compile-time string literal; caller must not free or modify it

## Dependency Status

- Project-local dependencies:
- `macro:note.h:40:NOTE_C_STRINGIZE` finalized in this slice
- External/system dependencies: none
- Data/type/global dependencies: compile-time `N_CJSON_VERSION_MAJOR`, `N_CJSON_VERSION_MINOR`, and `N_CJSON_VERSION_PATCH` macros from `n_cjson.h`
- Artifact coverage: implementation, header declaration, macro dependency, and direct source references inspected

## Behavior

Returns the CJSON version string built at compile time from `N_CJSON_VERSION_MAJOR`, `N_CJSON_VERSION_MINOR`, and `N_CJSON_VERSION_PATCH` using `NOTE_C_STRINGIZE`, currently `"1.7.7"`. The surrounding compile-time guard in `n_cjson.c:106`-`n_cjson.c:108` fails the build if the implementation and header version macros disagree.

## Important Boundaries

- Preconditions and assumptions: version macros are defined consistently with the implementation compile-time guard.
- Error/null/empty behavior: no runtime error path; mismatched version macros are a compile-time error.
- Ownership and lifetime: returns a string literal with static storage duration; no allocation.
- Concurrency/global state: no runtime state.
- Portability/platform constraints: public export/calling convention depends on `N_CJSON_PUBLIC`.
- Performance shape: O(1).
- Security or parsing constraints: no parsing; consumers should treat the returned pointer as read-only.

## Direct Call Sites

Exact source search found only:

- `n_cjson.h:180` public declaration.
- `n_cjson.c:110` definition.

No production source callers were found in this repository.

## Role In Callers

Public utility for external consumers that need to query the bundled CJSON compatibility version. It is not used by note-c production code.

## Tests Observed

No focused tests for `JVersion` were found.

## Graph Links

- Graph node id: `function:n_cjson.c:110:JVersion`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `NOTE_C_STRINGIZE`.

## Evidence Gaps

No unresolved source gaps for this node.
