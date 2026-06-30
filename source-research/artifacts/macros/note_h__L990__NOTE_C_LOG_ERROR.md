# `NOTE_C_LOG_ERROR`

## Status

Finalized. Macro definition, expansion behavior, debug-output callees, and production call-site groups were inspected from source.

## Identity

- Kind: macro
- Definition: `note.h:990`
- Declaration/interface: public header macro in `note.h`.
- Public/exported names: `NOTE_C_LOG_ERROR`, unless a consumer predefines it before this header section.
- Macro aliases/wrappers: optionally invokes `NOTE_C_LOG_FILE_AND_LINE`.
- Signature/API shape: `NOTE_C_LOG_ERROR(msg)`
- Visibility: available to translation units including `note.h`.
- Ownership/lifetime role: logs caller-provided message; does not allocate or own memory.

## Dependency Status

- Project-local dependencies:
- `macro:note.h:982:NOTE_C_LOG_FILE_AND_LINE` when `NOTE_C_LOG_SHOW_FILE_AND_LINE` is enabled.
- `function:n_hooks.c:485:NoteDebugWithLevel`.
- `function:n_hooks.c:499:NoteDebugWithLevelLn`.
- External/system dependencies: none directly.
- Data/type/global dependencies: log-level constants, debug hook state through the debug functions.
- Artifact coverage: generated graph originally missed macro-expansion callees; source-inspected edges are recorded in `inventory/dependency-graph.json`.

## Behavior

Unless already defined by a consumer, `NOTE_C_LOG_ERROR(msg)` expands to a `do { ... } while (0)` block:

1. Optionally emits file and line through `NOTE_C_LOG_FILE_AND_LINE(NOTE_C_LOG_LEVEL_ERROR)`.
2. Calls `NoteDebugWithLevel(NOTE_C_LOG_LEVEL_ERROR, "[ERROR] ")`.
3. Calls `NoteDebugWithLevelLn(NOTE_C_LOG_LEVEL_ERROR, msg)`.

The debug functions then enforce the configured log level and eventually dispatch to the registered debug-output hook when debugging is enabled.

## Important Boundaries

- Preconditions and assumptions: `msg` should be valid for the debug-output hook; debug functions tolerate disabled debug output.
- Error/null/empty behavior: no error return; logging can be dropped by log level or absent debug hook.
- Ownership and lifetime: no ownership transfer.
- Concurrency/global state: depends on global log level and debug-output hook.
- Portability/platform constraints: consumer can override the macro by defining `NOTE_C_LOG_ERROR` before this definition.
- Performance shape: compile-time macro plus two debug calls, plus optional file/line call.
- Security or parsing constraints: may emit file paths and line numbers when `NOTE_C_LOG_SHOW_FILE_AND_LINE` is enabled.

## Direct Call Sites

Exact production search found high-fanout error logging in:

- Definition: `note.h:990`.
- Request path: `n_request.c:71`, `n_request.c:311`, `n_request.c:432`, `n_request.c:453`, `n_request.c:469`, `n_request.c:681`.
- Serial path: `n_serial.c:50`, `n_serial.c:88`, `n_serial.c:160`, `n_serial.c:206`, `n_serial.c:247`.
- Helper/binary path: `n_helpers.c:102`, `n_helpers.c:134`, `n_helpers.c:176`, `n_helpers.c:254`, `n_helpers.c:365`, `n_helpers.c:437`, `n_helpers.c:545`, `n_helpers.c:699`, `n_helpers.c:1790`.
- I2C path: `n_i2c.c:57`, `n_i2c.c:119`, `n_i2c.c:207`, `n_i2c.c:295`, `n_i2c.c:430`, `n_i2c.c:495`.

## Role In Callers

Callers use this macro for cross-cutting request, transport, and helper error diagnostics. The macro itself only emits diagnostics; it does not alter caller control flow except by evaluating `msg`.

## Tests Observed

None. Source definition and call-site grouping were sufficient.

## Graph Links

- Graph node id: `macro:note.h:990:NOTE_C_LOG_ERROR`
- Structured graph: `inventory/dependency-graph.json`
- Callee artifacts: `artifacts/macros/note_h__L982__NOTE_C_LOG_FILE_AND_LINE.md`, `artifacts/functions/n_hooks_c__L485__NoteDebugWithLevel.md`, `artifacts/functions/n_hooks_c__L499__NoteDebugWithLevelLn.md`.
- Caller artifact candidates: high-fanout request, helper/binary, serial, and I2C error paths.
- Upstream public behavior candidates: most APIs that log request/transport/helper failures.

## Evidence Gaps

No unresolved macro expansion behavior. Individual debug function artifacts and high-fanout caller artifacts remain preliminary.
