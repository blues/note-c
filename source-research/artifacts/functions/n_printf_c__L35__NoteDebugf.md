# `NoteDebugf`

## Status

Finalized. Source-inspected public formatted debug-output helper.

## Identity

- Kind: function
- Definition: `n_printf.c:35`
- Declaration/interface: `note.h:951`
- Public/exported names: `NoteDebugf`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteDebugf(const char *format, ...)`
- Visibility: public note-c debug API implemented in `n_printf.c`
- Ownership/lifetime role: formats caller-provided arguments into a stack buffer and passes it to the configured debug hook

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:308:_noteIsDebugOutputActive` finalized
- External/system dependencies: libc `va_start`, `vsnprintf`, and `va_end`; dynamic dispatch through `hookDebugOutput`
- Data/type/global dependencies: reads global `hookDebugOutput`; compiled out under `NOTE_NODEBUG`
- Artifact coverage: source-inspected artifact

## Behavior

When `NOTE_NODEBUG` is not defined, checks whether debug output is active. If so, formats the variadic arguments into a local `char line[256]` with `vsnprintf` and calls `hookDebugOutput(line)`. When no debug hook is active, it returns without formatting. In `NOTE_NODEBUG` builds it casts `format` to void and does nothing.

## Important Boundaries

- Preconditions and assumptions: `format` must be valid for `vsnprintf` when a debug hook is active, and variadic arguments must match it.
- Error/null/empty behavior: no return value or error path. Hook return value is ignored. Null/invalid `format` is not guarded when formatting occurs.
- Ownership and lifetime: formatted output is stack-backed and valid only during the hook call.
- Concurrency/global state: reads/calls `hookDebugOutput` without locking; another thread can mutate the hook concurrently.
- Portability/platform constraints: `NOTE_NODEBUG` makes this API a no-op; otherwise it depends on libc `vsnprintf`.
- Performance shape: fixed 256-byte stack buffer and one formatting pass only when debug output is active.
- Security or parsing constraints: output is silently truncated to 255 characters plus terminator.

## Direct Call Sites

Production-only exact search found only the public declaration in `note.h:951` and the definition in `n_printf.c:35`. HITL support calls it from `test/hitl/card.binary/lib/notecard_binary/NotecardBinary.h`. No production repository caller, wrapper, or alias was found.

## Role In Callers

No production repository caller role. Public helper for downstream/debug code that wants printf-style debug output through the configured hook.

## Tests Observed

Focused tests in `test/src/NoteDebugf_test.cpp` cover hook set, formatted output, hook unset, and `NOTE_NODEBUG` behavior. Gaps include truncation, null format, hook mutation races, and hook failure return handling.

## Graph Links

- Graph node id: `function:n_printf.c:35:NoteDebugf`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_noteIsDebugOutputActive`; dynamic call to `hookDebugOutput` is source-inspected.

## Evidence Gaps

No unresolved source gaps for this node.
