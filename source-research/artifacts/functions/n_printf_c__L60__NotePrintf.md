# `NotePrintf`

## Status

Finalized. Source-inspected public printf-style print wrapper.

## Identity

- Kind: function
- Definition: `n_printf.c:60`
- Declaration/interface: `note.h:1134`
- Signature/API shape: `bool NotePrintf(const char *format, ...)`
- Visibility: public SDK API declared in `note.h`; definition is excluded when `NOTE_LOMEM` is defined
- Public/exported names: `NotePrintf`
- Macro aliases/wrappers: none
- Ownership/lifetime role: formats into a stack buffer and routes through `NotePrint`

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:847:NotePrint` finalized in this slice
- External/system dependencies: `va_start`, `va_end`, `vsnprintf`
- Data/type/global dependencies: 256-byte stack buffer
- Artifact coverage: generated artifact exists

## Behavior

When `NOTE_LOMEM` is not defined, formats variadic arguments into a local `char line[256]` with `vsnprintf`, then returns `NotePrint(line)`.

## Important Boundaries

- Preconditions and assumptions: `format` must be a valid printf-style format string for the supplied arguments.
- Error/null/empty behavior: does not inspect `vsnprintf` return value; truncation or formatting errors still pass the stack buffer to `NotePrint`.
- Ownership and lifetime: no heap allocation; formatted text is stack-local and consumed synchronously by `NotePrint`.
- Concurrency/global state: inherits debug hook and request/transport behavior from `NotePrint`.
- Portability/platform constraints: unavailable at the implementation level under `NOTE_LOMEM`; uses standard `vsnprintf`.
- Performance shape: bounded 256-byte formatting plus one `NotePrint` call.
- Security or parsing constraints: printf-format correctness is caller responsibility.

## Direct Call Sites

No in-repository production call sites were found beyond the definition and public declaration in `note.h:1134`.

## Role In Callers

This is the public formatted-output wrapper over `NotePrint`.

## Tests Observed

Focused `test/src/NotePrintf_test.cpp` fakes `NotePrint` and verifies printf-style formatting for `"Hello %s!"`.

## Graph Links

- Graph node id: `function:n_printf.c:60:NotePrintf`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `NotePrint`.

## Evidence Gaps

No unresolved source gaps for this node.
