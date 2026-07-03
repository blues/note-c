# `NoteI2CAddress`

## Status

Final. Behavior, public declaration, macro alias, global dependency, direct call sites, and focused tests have been inspected.

## Identity

- Kind: function
- Definition: `n_hooks.c:969`
- Declaration/interface: `note.h:1088`
- Public/exported names: `NoteI2CAddress`
- Macro aliases/wrappers: `_I2CAddress` expands to `NoteI2CAddress` at `n_lib.h:247`
- Signature/API shape: `uint32_t NoteI2CAddress(void)`
- Visibility: public SDK accessor and internal macro target
- Ownership/lifetime role: reads current I2C address state; no ownership

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none
- Data/type/global dependencies: reads `i2cAddress`; uses `NOTE_I2C_ADDR_DEFAULT`
- Artifact coverage: generated artifact exists

## Behavior

Returns `NOTE_I2C_ADDR_DEFAULT` when `i2cAddress == 0`; otherwise returns the stored `i2cAddress`.

## Important Boundaries

- Preconditions and assumptions: `i2cAddress` is maintained by I2C setter APIs
- Error/null/empty behavior: zero address is normalized to the default address, not returned as zero
- Ownership and lifetime: no allocation or ownership
- Concurrency/global state: reads global address state without locking
- Portability/platform constraints: default address is compile-time SDK constant
- Performance shape: constant-time branch
- Security or parsing constraints: no parsing

## Direct Call Sites

Direct internal macro callers use `_I2CAddress` in `n_i2c.c` and `n_request.c`. Focused direct tests are in `test/src/NoteI2CAddress_test.cpp`; reset tests also compare I2C hook arguments to `NoteI2CAddress()`.

## Role In Callers

Accessor used by I2C transaction/reset/chunk paths to pass the effective address to platform hooks.

## Tests Observed

`test/src/NoteI2CAddress_test.cpp` verifies zero returns `NOTE_I2C_ADDR_DEFAULT` and nonzero state is returned unchanged.

## Graph Links

- Graph node id: `function:n_hooks.c:969:NoteI2CAddress`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
