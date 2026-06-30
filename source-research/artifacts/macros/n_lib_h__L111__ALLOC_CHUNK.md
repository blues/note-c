# `ALLOC_CHUNK`

## Status

Finalized. Source-inspected definition, compile/config branch, direct references, graph links, and focused test evidence where available.

## Identity

- Kind: macro
- Definition: `n_lib.h:111`
- Declaration/interface: allocation chunk macro in `n_lib.h`
- Public/exported names: not a public function; visible through included headers or local translation-unit preprocessing.
- Macro aliases/wrappers: literal `64` bytes; `NOTE_C_LOW_MEM` branch.
- Signature/API shape: preprocessor constant or wrapper macro
- Visibility: I/O and cJSON allocation paths including `n_lib.h`.
- Ownership/lifetime role: no ownership; compile-time value only.

## Dependency Status

- Project-local dependencies: No project-local callees; this macro expands to a literal, alias, or compile-time branch.
- External/system dependencies: none.
- Data/type/global dependencies: compile configuration and direct callers as listed below.
- Artifact coverage: source-inspected artifact exists.

## Behavior

Sets the allocation growth quantum for response buffers and cJSON print buffers in the `NOTE_C_LOW_MEM` branch. I2C and serial receive buffers round up or grow by multiples of this value; cJSON print reallocation also rounds needed size to a chunk multiple.

## Important Boundaries

- Compile-time configuration branch; only one `ALLOC_CHUNK` definition is active.
- Callers add space for null termination where JSON parsing requires it.
- Smaller low-memory branch trades allocation granularity for constrained targets.

## Direct Call Sites

Used by `_i2cNoteTransaction`, `_i2cNoteReset`, `_serialNoteTransaction`, and `_ensure` in `n_cjson.c`.

## Role In Callers

Reduces heap fragmentation by growing buffers in fixed chunks while keeping the SDK portable across memory profiles.

## Tests Observed

Focused transport and cJSON print tests exercise allocation/growth paths; no test asserts the exact chunk literal independently.

## Graph Links

- Graph node id: `macro:n_lib.h:111:ALLOC_CHUNK`
- Structured graph: `inventory/dependency-graph.json`
- Verified `uses_constant` edges from the active allocation users to this compile branch.

## Evidence Gaps

No unresolved source gaps for this node.
