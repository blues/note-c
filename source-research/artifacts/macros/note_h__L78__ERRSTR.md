# `ERRSTR`

## Status

Finalized. Conditional macro definitions, compile-time variant behavior, and production call-site groups were inspected from source.

## Identity

- Kind: macro
- Definition: `note.h:78`
- Declaration/interface: public header macro included through `note.h`.
- Public/exported names: `ERRSTR`.
- Macro aliases/wrappers: conditional counterpart at `note.h:75`.
- Signature/API shape: `ERRSTR(x, y)`
- Visibility: available to all translation units including `note.h`; this node is the normal non-low-memory definition.
- Ownership/lifetime role: selects between existing string expressions; does not allocate or own memory.

## Dependency Status

- Project-local dependencies:
- No project-local callees found by generated scan.
- External/system dependencies: none.
- Data/type/global dependencies: depends on compile-time absence of `NOTE_C_LOW_MEM`.
- Artifact coverage: paired low-memory definition artifact exists at `artifacts/macros/note_h__L75__ERRSTR.md`.

## Behavior

Without `NOTE_C_LOW_MEM`, `ERRSTR(x, y)` expands to `(x)`, selecting verbose string literals or dynamic string expressions. Under `NOTE_C_LOW_MEM`, the paired definition at `note.h:75` expands to `(y)`, selecting compact constant strings such as `c_bad`, `c_err`, `c_mem`, or transport error constants.

## Important Boundaries

- Preconditions and assumptions: both arguments must be valid in the compiling context; only one is selected by preprocessing.
- Error/null/empty behavior: no runtime behavior; selected expression may be NULL only if caller passes NULL.
- Ownership and lifetime: no ownership changes; most uses select string literals or global constant strings.
- Concurrency/global state: no runtime state.
- Portability/platform constraints: compile-time low-memory variant; `NOTE_LOWMEM` also defines `NOTE_C_LOW_MEM`.
- Performance shape: compile-time selection, no runtime cost.
- Security or parsing constraints: normal builds retain verbose diagnostic strings and therefore larger string footprint.

## Direct Call Sites

Exact production search found `ERRSTR` used in request, serial, helper/binary, and I2C error/logging paths:

- Definitions: `note.h:75`, `note.h:78`.
- Request path: `n_request.c:311`, `n_request.c:432`, `n_request.c:467`, `n_request.c:539`, `n_request.c:620`, `n_request.c:632`, `n_request.c:688`.
- Serial path: `n_serial.c:72`, `n_serial.c:87`, `n_serial.c:160`, `n_serial.c:206`, `n_serial.c:247`.
- Helper/binary path: `n_helpers.c:102`, `n_helpers.c:134`, `n_helpers.c:176`, `n_helpers.c:253`, `n_helpers.c:364`, `n_helpers.c:431`, `n_helpers.c:544`, `n_helpers.c:692`.
- I2C path: `n_i2c.c:63`, `n_i2c.c:119`, `n_i2c.c:207`, `n_i2c.c:413`, `n_i2c.c:432`.

The exact search had high fanout; representative grouped locations above cover all production files containing the macro.

## Role In Callers

Callers use `ERRSTR` to compile either verbose diagnostics or compact global constants into logging and returned error values. This is a cross-cutting low-memory boundary rather than an independent runtime function.

## Tests Observed

None. Source definitions and call-site grouping were sufficient.

## Graph Links

- Graph node id: `macro:note.h:78:ERRSTR`
- Structured graph: `inventory/dependency-graph.json`
- Callee artifacts: none.
- Caller artifact candidates: high-fanout request, helper/binary, serial, and I2C error paths.
- Upstream public behavior candidates: most APIs that return `const char *` error strings or log transport/request failures.

## Evidence Gaps

No unresolved macro behavior. Individual high-fanout caller artifacts remain preliminary.
