# `NoteDebugIntln`

## Status

Finalized. Source-inspected public debug integer line emitter.

## Identity

- Kind: function
- Definition: `n_hooks.c:453`
- Declaration/interface: `note.h:944`
- Public/exported names: `NoteDebugIntln`; internal macro alias `_DebugIntln(x, y)` expands to this function when `NOTE_NODEBUG` is not defined and to a no-op when it is defined
- Signature/API shape: `void NoteDebugIntln(const char *msg, int n)`
- Visibility: public note-c debug API implemented in `n_hooks.c`
- Ownership/lifetime role: emits caller-provided message text, formatted integer text, and newline through the configured debug output path

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:256:_Debug` finalized
  - `function:n_cjson_helpers.c:284:JItoA` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads `c_newline`; transitive debug hook and log-level state through `_Debug`/`NoteDebug`
- Artifact coverage: source-inspected artifact

## Behavior

If `msg` is non-null, emits it through `_Debug`. It then converts `n` into a local `char str[16]` buffer with `JItoA`, emits the converted number through `_Debug`, and emits `c_newline` through `_Debug`. In `NOTE_NODEBUG` builds the `_Debug` macro is a no-op, but the function body still performs the integer conversion when called directly.

## Important Boundaries

- Preconditions and assumptions: non-null `msg` must point to a valid C string for the debug hook path.
- Error/null/empty behavior: null `msg` skips only the message prefix; the integer and newline are still processed. There is no return value or error path.
- Ownership and lifetime: no ownership transfer; text pointers are consumed synchronously by debug output.
- Concurrency/global state: output depends on global `hookDebugOutput` and debug/log configuration reached through `_Debug`.
- Portability/platform constraints: `_DebugIntln` macro calls are compiled out under `NOTE_NODEBUG`; direct calls to `NoteDebugIntln` still compile.
- Performance shape: optional debug emit, integer-to-string conversion, two further debug emits.
- Security or parsing constraints: integer formatting uses a fixed 16-byte local buffer through finalized `JItoA`.

## Direct Call Sites

Source/header exact search found the public declaration in `note.h:944`, macro alias definition in `n_lib.h`, the definition in `n_hooks.c:453`, and focused tests. No production repository caller of `NoteDebugIntln` or `_DebugIntln` was found.

## Role In Callers

No production repository caller role. Public and macro-backed helper for downstream/debug code that wants to print a message, integer, and newline.

## Tests Observed

Focused assertions in `test/src/NoteDebug_test.cpp` cover null message, non-null message, and `NOTE_NODEBUG` behavior. Gaps include negative integers, multi-digit edge cases, minimum integer formatting, and direct no-hook behavior.

## Graph Links

- Graph node id: `function:n_hooks.c:453:NoteDebugIntln`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_Debug` and `JItoA`.

## Evidence Gaps

No unresolved source gaps for this node.
