# `NoteSetUserAgentCPU`

## Status

Finalized. Source-inspected public user-agent CPU metadata setter.

## Identity

- Kind: function
- Definition: `n_ua.c:214`
- Declaration/interface: `note.h:921`
- Public/exported names: `NoteSetUserAgentCPU`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetUserAgentCPU(int cpu_mem, int cpu_mhz, int cpu_cores, char *cpu_vendor, char *cpu_name)`
- Visibility: public note-c API when `n_ua.c` is built; excluded from low-memory builds by source/build configuration
- Ownership/lifetime role: stores caller-provided CPU values and string pointers for later user-agent JSON construction

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes file-static `n_cpu_cores`, `n_cpu_mem`, `n_cpu_mhz`, `n_cpu_name`, and `n_cpu_vendor`
- Artifact coverage: source-inspected artifact

## Behavior

Assigns the CPU core count, memory, MHz, vendor string pointer, and CPU name string pointer into file-static user-agent state. It does not copy, validate, or allocate the strings.

## Important Boundaries

- Preconditions and assumptions: non-null strings, when provided, must remain valid until user-agent construction copies them.
- Error/null/empty behavior: zero numeric values and null strings are accepted and later suppress optional CPU fields in `NoteUserAgent`.
- Ownership and lifetime: caller owns all pointed-to strings; note-c stores only raw pointers.
- Concurrency/global state: writes shared file-static state without locking.
- Portability/platform constraints: implementation exists only outside `NOTE_C_LOW_MEM` builds; compile-time Arduino defaults may be overwritten by this setter.
- Performance shape: three scalar assignments and two pointer assignments.
- Security or parsing constraints: no string validation or lifetime protection.

## Direct Call Sites

Production-only exact search found only the public declaration in `note.h:921` and the definition in `n_ua.c:214`. Focused tests call it from `test/src/NoteUserAgent_test.cpp`. No production repository caller, wrapper, or alias was found.

## Role In Callers

No production repository caller role. Public setter for downstream libraries to stamp CPU metadata into `NoteUserAgent` output.

## Tests Observed

Focused tests in `test/src/NoteUserAgent_test.cpp` set CPU values and verify emitted fields. Gaps include zero suppression, null strings, Arduino default preservation/override, pointer lifetime after caller buffer mutation, and low-memory build behavior.

## Graph Links

- Graph node id: `function:n_ua.c:214:NoteSetUserAgentCPU`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
