# `NoteSetEnvDefaultInt`

## Status

Finalized. Source-inspected integer environment default wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1235`
- Declaration/interface: `note.h:1838`
- Signature/API shape: `bool NoteSetEnvDefaultInt(const char *variable, JINTEGER defaultVal)`
- Visibility: public SDK API
- Public/exported names: `NoteSetEnvDefaultInt`
- Macro aliases/wrappers: none
- Ownership/lifetime role: uses a stack string buffer and delegates to `NoteSetEnvDefault`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson_helpers.c:284:JItoA` finalized
  - `function:n_helpers.c:1215:NoteSetEnvDefault` finalized
- External/system dependencies: none
- Data/type/global dependencies: `JINTEGER`

## Behavior

Allocates a local `char buf[32]`, converts `defaultVal` into it with `JItoA(defaultVal, buf)`, and returns `NoteSetEnvDefault(variable, buf)`.

## Important Boundaries

- Preconditions and assumptions: `variable` is passed through to `NoteSetEnvDefault`; source has no local null guard.
- Error/null/empty behavior: conversion has no local error branch; delegated `NoteSetEnvDefault` result is returned unchanged.
- Ownership and lifetime: stack buffer is valid for the duration of the synchronous delegated call; no heap ownership.
- Concurrency/global state: none locally.
- Performance shape: O(number of integer digits) conversion plus delegated request.
- Portability/platform constraints: integer formatting is handled by note-c helper code.

## Direct Call Sites

Exact source search found public declaration, implementation, and focused tests. No production library caller was found; this is a public API entry point.

## Role In Callers

Provides integer formatting before using the shared string `env.default` request implementation.

## Tests Observed

`test/src/NoteSetEnvDefaultNumber_test.cpp` covers `NoteSetEnvDefaultInt` formatting `-123456` to `"-123456"` and delegation to a faked `NoteSetEnvDefault`.

## Graph Links

- Graph node id: `function:n_helpers.c:1235:NoteSetEnvDefaultInt`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
