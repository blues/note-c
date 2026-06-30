# `NoteSetI2CMtu`

## Status

Finalized. Source-inspected public raw I2C MTU setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:982`
- Declaration/interface: `note.h:804`
- Public/exported names: `NoteSetI2CMtu`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetI2CMtu(uint32_t i2cMtu)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: stores raw I2C MTU configuration

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes `i2cMax`
- Artifact coverage: source-inspected artifact

## Behavior

Assigns `i2cMax = i2cMtu`. Zero and oversized values are stored as raw values; the separate `NoteI2CMax` effective getter applies defaulting and clamping.

## Important Boundaries

- Preconditions and assumptions: scalar input only.
- Error/null/empty behavior: no return value or error path.
- Ownership and lifetime: no ownership transfer.
- Concurrency/global state: unsynchronized write to `i2cMax`.
- Portability/platform constraints: stores raw value; effective defaulting/clamping is elsewhere.
- Performance shape: one scalar assignment.
- Security or parsing constraints: no validation of MTU range.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:804`, the definition in `n_hooks.c:982`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public setter for downstream code that wants to override the stored I2C MTU.

## Tests Observed

Focused tests in `test/src/NoteSetI2CMtu_test.cpp` cover valid, zero, and oversized raw writes. Gap: no round trip through effective `NoteI2CMax` in that test.

## Graph Links

- Graph node id: `function:n_hooks.c:982:NoteSetI2CMtu`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
