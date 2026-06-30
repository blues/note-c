# `NoteGetEnvInt`

## Status

Finalized. Source-inspected public integer environment retrieval wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1281`
- Declaration/interface: `note.h:1791`
- Signature/API shape: `JINTEGER NoteGetEnvInt(const char *variable, JINTEGER defaultVal)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NoteGetEnvInt`
- Macro aliases/wrappers: none
- Ownership/lifetime role: stack-buffer wrapper around `NoteGetEnv`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson_helpers.c:284:JItoA` finalized
  - `function:n_helpers.c:1302:NoteGetEnv` finalized in this slice
- External/system dependencies: `atoi`
- Data/type/global dependencies: stack buffers sized 32 bytes
- Artifact coverage: generated artifact exists

## Behavior

Formats `defaultVal` into a default string with `JItoA(defaultVal, buf2)`, calls `NoteGetEnv(variable, buf2, buf, sizeof(buf))`, ignores the boolean result, and returns `atoi(buf)`.

## Important Boundaries

- Preconditions and assumptions: 32-byte buffers are expected to hold `JINTEGER` text and returned integer text.
- Error/null/empty behavior: because `NoteGetEnv` initializes `buf` to `buf2`, request failures and missing values normally parse back to `defaultVal`. Non-integer returned text follows `atoi` behavior, commonly `0`.
- Ownership and lifetime: no allocation; all intermediate storage is stack-owned.
- Concurrency/global state: reaches request/transport hooks through `NoteGetEnv`.
- Portability/platform constraints: `atoi` conversion semantics are C library dependent for overflow.
- Performance shape: bounded integer formatting/parsing plus one `NoteGetEnv` transaction.
- Security or parsing constraints: ignores `NoteGetEnv` success/failure and trusts parsed buffer contents.

## Direct Call Sites

No in-repository production call sites were found beyond the definition and public declaration in `note.h:1791`.

## Role In Callers

This is the public integer convenience API over `NoteGetEnv`.

## Tests Observed

Focused `test/src/NoteGetEnvNumber_test.cpp` fakes `NoteGetEnv` and verifies existing values parse to the returned integer while misses/failures parse the default string.

## Graph Links

- Graph node id: `function:n_helpers.c:1281:NoteGetEnvInt`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JItoA` and `NoteGetEnv`.

## Evidence Gaps

No unresolved source gaps for this node.
