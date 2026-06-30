# `NoteSetUserAgentOS`

## Status

Finalized. Source-inspected public user-agent OS metadata setter.

## Identity

- Kind: function
- Definition: `n_ua.c:201`
- Declaration/interface: `note.h:911`
- Public/exported names: `NoteSetUserAgentOS`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetUserAgentOS(char *os_name, char *os_platform, char *os_family, char *os_version)`
- Visibility: public note-c API when `n_ua.c` is built; excluded from low-memory builds by source/build configuration
- Ownership/lifetime role: stores caller-provided OS metadata string pointers for later user-agent JSON construction

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes file-static `n_os_family`, `n_os_name`, `n_os_platform`, and `n_os_version`
- Artifact coverage: source-inspected artifact

## Behavior

Assigns the four OS metadata globals from the supplied pointers. It does not copy, validate, or allocate the strings.

## Important Boundaries

- Preconditions and assumptions: non-null strings, when provided, must remain valid until user-agent construction copies them.
- Error/null/empty behavior: null values are accepted and later suppress their optional OS fields in `NoteUserAgent`.
- Ownership and lifetime: caller owns all pointed-to strings; note-c stores only raw pointers.
- Concurrency/global state: writes shared file-static state without locking.
- Portability/platform constraints: implementation exists only outside `NOTE_C_LOW_MEM` builds.
- Performance shape: four pointer assignments.
- Security or parsing constraints: no string validation or lifetime protection.

## Direct Call Sites

Production-only exact search found only the public declaration in `note.h:911` and the definition in `n_ua.c:201`. Focused tests call it from `test/src/NoteUserAgent_test.cpp`. No production repository caller, wrapper, or alias was found.

## Role In Callers

No production repository caller role. Public setter for downstream libraries to stamp OS metadata into `NoteUserAgent` output.

## Tests Observed

Focused tests in `test/src/NoteUserAgent_test.cpp` set all OS values and verify emitted fields. Gaps include partial nulls, pointer lifetime after caller buffer mutation, and low-memory build behavior.

## Graph Links

- Graph node id: `function:n_ua.c:201:NoteSetUserAgentOS`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
