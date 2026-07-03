# `NoteGetI2CAddress`

## Status

Finalized. Source-inspected public raw I2C address getter.

## Identity

- Kind: function
- Definition: `n_hooks.c:835`
- Declaration/interface: `note.h:810`
- Public/exported names: `NoteGetI2CAddress`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteGetI2CAddress(uint32_t *i2cAddr)`
- Visibility: public note-c hook API implemented in `n_hooks.c`
- Ownership/lifetime role: copies the raw stored I2C address into caller-provided storage

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: reads `i2cAddress`
- Artifact coverage: source-inspected artifact

## Behavior

If `i2cAddr` is non-null, writes the raw stored `i2cAddress` value to it. It does not lock and does not apply the default address logic used by `NoteI2CAddress`.

## Important Boundaries

- Preconditions and assumptions: non-null output pointer must point to writable `uint32_t` storage.
- Error/null/empty behavior: null output pointer is a no-op; no return value or error path.
- Ownership and lifetime: no ownership transfer.
- Concurrency/global state: unsynchronized read of `i2cAddress`.
- Portability/platform constraints: reports raw stored value, not effective I2C address.
- Performance shape: one null check and optional scalar copy.
- Security or parsing constraints: no validation.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:810`, the definition in `n_hooks.c:835`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public getter for downstream code that wants the raw configured I2C address.

## Tests Observed

Focused tests in `test/src/NoteGetI2CAddress_test.cpp` cover raw zero, raw set value, and null output pointer. Gap: no locking/concurrency coverage.

## Graph Links

- Graph node id: `function:n_hooks.c:835:NoteGetI2CAddress`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf.

## Evidence Gaps

No unresolved source gaps for this node.
