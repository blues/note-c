# `NoteSetI2CAddress`

## Status

Finalized. Source-inspected public raw I2C address setter.

## Identity

- Kind: function
- Definition: `n_hooks.c:977`
- Declaration/interface: `note.h:795`
- Public/exported names: `NoteSetI2CAddress`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteSetI2CAddress(uint32_t i2cAddr)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: stores raw I2C address configuration

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes `i2cAddress`
- Artifact coverage: source-inspected artifact

## Behavior

Assigns `i2cAddress = i2cAddr`. Zero is stored as a raw value; the separate `NoteI2CAddress` effective getter maps zero to the default address.

## Important Boundaries

- Preconditions and assumptions: scalar input only.
- Error/null/empty behavior: no return value or error path.
- Ownership and lifetime: no ownership transfer.
- Concurrency/global state: unsynchronized write to `i2cAddress`.
- Portability/platform constraints: stores raw value; effective defaulting is elsewhere.
- Performance shape: one scalar assignment.
- Security or parsing constraints: no validation of address range.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:795`, the definition in `n_hooks.c:977`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public setter for downstream code that wants to override the stored I2C address.

## Tests Observed

Focused tests in `test/src/NoteSetI2CAddress_test.cpp` cover nonzero and zero raw writes. Gap: no round trip through effective `NoteI2CAddress` in that test.

## Graph Links

- Graph node id: `function:n_hooks.c:977:NoteSetI2CAddress`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
