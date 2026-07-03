# `strlcat`

## Status

Finalized. Source-inspected portable fallback string-concatenation helper, build gating, declarations, and direct production callers.

## Identity

- Kind: function
- Definition: `n_str.c:69` for MSVC, `n_str.c:71` as a weak symbol otherwise
- Declaration/interface: `note.h:1172` for C++ and `note.h:1174` for C, both inside `#ifndef HAVE_STRLCAT`
- Public/exported names: `strlcat` when the fallback is compiled
- Macro aliases/wrappers: none
- Signature/API shape: `size_t strlcat(char *dst, const char *src, size_t siz)`
- Visibility: SDK fallback symbol compiled when the platform lacks `strlcpy` or `strlcat`
- Ownership/lifetime role: appends into caller-owned destination buffer and allocates nothing

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `strlen`
- Data/type/global dependencies: mutates caller-provided destination buffer; no global state
- Build/config dependencies:
  - `CMakeLists.txt:50` checks for platform `strlcat`.
  - `CMakeLists.txt:68`-`CMakeLists.txt:72` appends `n_str.c` only when `HAVE_STRLCPY` or `HAVE_STRLCAT` is missing.
  - `note.h:1172`/`note.h:1174` declare the fallback only when `HAVE_STRLCAT` is not defined.
- Artifact coverage: implementation, declaration guards, build gating, direct source callers, and graph node inspected

## Behavior

Finds the end of the existing NUL-terminated `dst` string without scanning past `siz` bytes. If `dst` already occupies the whole destination size, it appends nothing and returns `siz + strlen(src)` through the `dlen + strlen(s)` expression. Otherwise it appends as much of `src` as fits while reserving one byte for the terminator, NUL-terminates the result, and returns the length the concatenation would have had without truncation: `initial_dst_len + strlen(src)`.

## Important Boundaries

- Preconditions and assumptions: caller must pass valid string pointers; `dst` must be writable for `siz` bytes when appending is possible; both input strings must be NUL-terminated.
- Error/null/empty behavior: no null checks. If `siz <= strlen(initial dst)`, no terminator is written and the return value still includes the full source length for truncation detection.
- Ownership and lifetime: no allocation or ownership transfer; mutates only the caller destination buffer.
- Concurrency/global state: no global state.
- Portability/platform constraints: fallback is weak except on MSVC; CMake omits `n_str.c` on platforms with native `strlcpy`/`strlcat` to avoid fortified macro conflicts.
- Performance shape: O(min(siz, strlen(dst)) + strlen(src)).
- Security or parsing constraints: bounded append prevents writes past `siz`, but callers must still check the return value when truncation matters.

## Direct Call Sites

Exact source search `rg -n "\bstrlcat\b" -g '!source-research/**' -g '!docs/**'` found:

- Build/config and declarations: `CMakeLists.txt:50`, `CMakeLists.txt:68`-`CMakeLists.txt:72`, `note.h:1172`, `note.h:1174`, `n_str.c:69`/`n_str.c:71`.
- `n_helpers.c:1778` and `n_helpers.c:1779` `NoteSleep` appends `","` and caller-provided sleep modes to the local `"sleep"` mode string.
- `n_helpers.c:2152` `NoteSendToRoute` appends a caller-provided method to the local `"web."` request string.

## Role In Callers

Callers use `strlcat` as the bounded append companion to `strlcpy` when constructing small stack request strings. Current production callers do not inspect the return value, so truncation silently produces a bounded but shortened mode or request name.

## Tests Observed

No standalone focused tests for the fallback `strlcat` implementation were found. `test/src/NoteRequestResponseJSON_test.cpp` uses `strlcat` as test setup support, but that is not coverage of this fallback symbol's edge cases.

## Graph Links

- Graph node id: `function:n_str.c:69:strlcat`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming edges from `NoteSleep` and `NoteSendToRoute`.

## Evidence Gaps

No unresolved source gaps for this node. Platform-native `strlcat` behavior is represented as a build/config variant rather than implementation source in this repository.
