# `NoteGetI2CMtu`

## Status

Finalized. Source-inspected public raw I2C MTU getter.

## Identity

- Kind: function
- Definition: `n_hooks.c:842`
- Declaration/interface: `note.h:819`
- Public/exported names: `NoteGetI2CMtu`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteGetI2CMtu(uint32_t *i2cMtu)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: copies the raw stored I2C MTU into caller-provided storage

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `i2cMax`
- Artifact coverage: source-inspected artifact

## Behavior

If `i2cMtu` is non-null, writes the raw stored `i2cMax` value to it. It does not lock and does not apply the default/clamp logic used by `NoteI2CMax`.

## Important Boundaries

- Preconditions and assumptions: non-null output pointer must point to writable `uint32_t` storage.
- Error/null/empty behavior: null output pointer is a no-op; no return value or error path.
- Ownership and lifetime: no ownership transfer.
- Concurrency/global state: unsynchronized read of `i2cMax`.
- Portability/platform constraints: reports raw stored MTU, not effective clamped MTU.
- Performance shape: one null check and optional scalar copy.
- Security or parsing constraints: no validation.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:819`, the definition in `n_hooks.c:842`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public getter for downstream code that wants the raw configured I2C MTU.

## Tests Observed

Focused tests in `test/src/NoteGetI2CMtu_test.cpp` cover raw zero, raw oversized, valid value, and null output pointer. Gap: no locking/concurrency coverage.

## Graph Links

- Graph node id: `function:n_hooks.c:842:NoteGetI2CMtu`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
