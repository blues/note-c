# `NOTE_C_SHOW_MALLOC`

## Status

Finalized. Source-inspected malloc/free debug logging compile-time switch.

## Identity

- Kind: macro
- Definition: `n_hooks.c:33`
- Declaration/interface: file-local default guarded by `#ifndef NOTE_C_SHOW_MALLOC`
- Public/exported names: build option `NOTE_C_SHOW_MALLOC`
- Macro aliases/wrappers: CMake option of the same name adds a compile definition when enabled
- Signature/API shape: `#define NOTE_C_SHOW_MALLOC  false`
- Visibility: compile-time configuration macro consumed in `n_hooks.c`
- Ownership/lifetime role: controls whether malloc/free hook operations emit debug messages

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `static_assert` and pointer-size assumption in the enabled branch; `string.h` is included when enabled
- Data/type/global dependencies: guards `_n_ptoa32`, `NoteMalloc` logging, and `NoteFree` logging
- Artifact coverage: source-inspected artifact with verified incoming compile-guard edges

## Behavior

Defaults to false unless supplied by the build. When enabled, `n_hooks.c` includes extra code that logs `malloc` and `free` hook activity through `hookDebugOutput` when debug output is active. The malloc path logs requested size, failure state, and pointer value; the free path logs the pointer value. In `NOTE_C_LOW_MEM` builds, the `NoteMalloc`/`NoteFree` logging blocks are suppressed even if the macro is set.

## Important Boundaries

- Preconditions and assumptions: enabled pointer logging statically asserts `sizeof(void *) == sizeof(uint32_t)` and converts pointers through `_n_ptoa32`.
- Error/null/empty behavior: not a runtime function; when disabled, logging code is not compiled.
- Ownership and lifetime: does not change allocation/free ownership semantics.
- Concurrency/global state: reads debug hook state and emits through `hookDebugOutput` only when active.
- Portability/platform constraints: enabled pointer logging assumes 32-bit pointers, which can reject builds on wider-pointer platforms.
- Performance shape: disabled branch compiles out; enabled branch adds debug checks, integer/pointer formatting, and hook output calls.
- Security or parsing constraints: enabled logging exposes allocation sizes and pointer values to the configured debug output.

## Direct Call Sites

Exact source search found the default definition, CMake option/compile-definition handling, the include guard for `string.h`, the conditional `_n_ptoa32` helper, and conditional blocks in `NoteMalloc` and `NoteFree`.

## Role In Callers

`NoteMalloc` and `NoteFree` keep their allocation semantics but optionally emit diagnostic allocation traces when this compile-time switch is active and low-memory mode is not active.

## Tests Observed

No focused source test references were found outside test documentation prose. Gap: no focused test asserts enabled logging output, disabled compilation, low-memory suppression, or the 32-bit pointer-size guard.

## Graph Links

- Graph node id: `macro:n_hooks.c:33:NOTE_C_SHOW_MALLOC`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming `uses_constant` edges from `_n_ptoa32`, `NoteMalloc`, and `NoteFree`.

## Evidence Gaps

No unresolved source gaps for this node.
