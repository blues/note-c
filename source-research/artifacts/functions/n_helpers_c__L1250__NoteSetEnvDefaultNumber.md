# `NoteSetEnvDefaultNumber`

## Status

Finalized. Source-inspected numeric environment default wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1250`
- Declaration/interface: `note.h:1829`
- Signature/API shape: `bool NoteSetEnvDefaultNumber(const char *variable, JNUMBER defaultVal)`
- Visibility: public SDK API
- Public/exported names: `NoteSetEnvDefaultNumber`
- Macro aliases/wrappers: none
- Ownership/lifetime role: uses a stack string buffer and delegates to `NoteSetEnvDefault`

## Dependency Status

- Project-local dependencies:
  - `function:n_ftoa.c:56:JNtoA` finalized
  - `function:n_helpers.c:1215:NoteSetEnvDefault` finalized
- External/system dependencies: none
- Data/type/global dependencies: `JNUMBER`, `JNTOA_MAX`

## Behavior

Allocates a local `char buf[JNTOA_MAX]`, converts `defaultVal` into it with `JNtoA(defaultVal, buf, -1)`, and returns `NoteSetEnvDefault(variable, buf)`.

Passing precision `-1` selects the default number-to-string behavior implemented by `JNtoA`.

## Important Boundaries

- Preconditions and assumptions: `variable` is passed through to `NoteSetEnvDefault`; source has no local null guard.
- Error/null/empty behavior: conversion has no local error branch; delegated `NoteSetEnvDefault` result is returned unchanged.
- Ownership and lifetime: stack buffer is valid for the duration of the synchronous delegated call; no heap ownership.
- Concurrency/global state: none locally.
- Performance shape: numeric formatting plus delegated request.
- Portability/platform constraints: number formatting is handled by note-c helper code.

## Direct Call Sites

Exact source search found public declaration, implementation, and focused tests. No production library caller was found; this is a public API entry point.

## Role In Callers

Provides floating/numeric formatting before using the shared string `env.default` request implementation.

## Tests Observed

`test/src/NoteSetEnvDefaultNumber_test.cpp` covers `NoteSetEnvDefaultNumber` formatting `-123456` to `"-123456"` and delegation to a faked `NoteSetEnvDefault`.

## Graph Links

- Graph node id: `function:n_helpers.c:1250:NoteSetEnvDefaultNumber`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
