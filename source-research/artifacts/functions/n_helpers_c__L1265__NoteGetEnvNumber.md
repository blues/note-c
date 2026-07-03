# `NoteGetEnvNumber`

## Status

Finalized. Source-inspected public numeric environment retrieval wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1265`
- Declaration/interface: `note.h:1800`
- Signature/API shape: `JNUMBER NoteGetEnvNumber(const char *variable, JNUMBER defaultVal)`
- Visibility: public SDK API declared in `note.h`
- Public/exported names: `NoteGetEnvNumber`
- Macro aliases/wrappers: none
- Ownership/lifetime role: stack-buffer wrapper around `NoteGetEnv`

## Dependency Status

- Project-local dependencies:
  - `function:n_ftoa.c:56:JNtoA` finalized
  - `function:n_helpers.c:1302:NoteGetEnv` finalized in this slice
  - `function:n_atof.c:71:JAtoN` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: stack buffers sized `JNTOA_MAX`
- Artifact coverage: generated artifact exists

## Behavior

Formats `defaultVal` into a default string with `JNtoA(defaultVal, buf2, -1)`, calls `NoteGetEnv(variable, buf2, buf, sizeof(buf))`, ignores the boolean result, and returns `JAtoN(buf, NULL)`.

## Important Boundaries

- Preconditions and assumptions: `JNTOA_MAX` buffers are large enough for formatted default and returned numeric text.
- Error/null/empty behavior: because `NoteGetEnv` initializes `buf` to `buf2`, request failures and missing values normally parse back to `defaultVal`. Non-numeric returned text parses according to `JAtoN` behavior, commonly `0.0`.
- Ownership and lifetime: no allocation; all intermediate storage is stack-owned.
- Concurrency/global state: reaches request/transport hooks through `NoteGetEnv`.
- Portability/platform constraints: `JNUMBER` precision depends on build configuration; formatting/parsing uses note-c helpers.
- Performance shape: bounded numeric formatting/parsing plus one `NoteGetEnv` transaction.
- Security or parsing constraints: ignores `NoteGetEnv` success/failure and trusts parsed buffer contents.

## Direct Call Sites

No in-repository production call sites were found beyond the definition and public declaration in `note.h:1800`.

## Role In Callers

This is the public numeric convenience API over `NoteGetEnv`.

## Tests Observed

Focused `test/src/NoteGetEnvNumber_test.cpp` fakes `NoteGetEnv` and verifies existing values parse to the returned number while misses/failures parse the default string.

## Graph Links

- Graph node id: `function:n_helpers.c:1265:NoteGetEnvNumber`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JNtoA`, `NoteGetEnv`, and `JAtoN`.

## Evidence Gaps

No unresolved source gaps for this node.
