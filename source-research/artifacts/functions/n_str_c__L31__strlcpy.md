# `strlcpy`

## Status

Finalized. Source-inspected portable fallback string-copy helper with grouped verified fanout.

## Identity

- Kind: function
- Definition: `n_str.c:31` for MSVC, `n_str.c:33` as a weak symbol otherwise
- Declaration/interface: `note.h:1152` for C++ and `note.h:1154` for C, both inside `#ifndef HAVE_STRLCPY`
- Signature/API shape: `size_t strlcpy(char *dst, const char *src, size_t siz)`
- Visibility: SDK fallback symbol compiled only when the platform lacks `strlcpy` or `strlcat`
- Public/exported names: `strlcpy` when fallback is compiled
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: primitive pointer/string traversal; `strlen` is not used by `strlcpy`
- Data/type/global dependencies: mutates caller-provided destination buffer; no global state
- Build/config dependencies:
  - `CMakeLists.txt:49` checks for platform `strlcpy`.
  - `CMakeLists.txt:68`-`CMakeLists.txt:72` appends `n_str.c` only when `HAVE_STRLCPY` or `HAVE_STRLCAT` is missing.
  - `note.h:1152`/`note.h:1154` declare the fallback only when `HAVE_STRLCPY` is not defined.
- Artifact coverage: implementation, declaration guards, build gating, direct source callers, and graph node inspected

## Behavior

Copies `src` into `dst` with total destination size `siz`. If `siz` is nonzero, it copies at most `siz - 1` bytes and stops early when it copies a NUL byte. If the destination fills before the source ends, it writes a terminating NUL when `siz != 0`, then advances through the rest of `src`. The return value is the length of `src`, excluding the NUL terminator, so callers can detect truncation when the return value is greater than or equal to `siz`.

## Important Boundaries

- Preconditions and assumptions: caller must pass valid string pointers; destination must be writable for `siz` bytes when `siz > 0`; source must be NUL-terminated.
- Error/null/empty behavior: no null checks. `siz == 0` performs no destination write but still traverses `src` to compute the return length.
- Ownership and lifetime: copies into caller-owned memory and allocates nothing.
- Concurrency/global state: no global state.
- Portability/platform constraints: fallback is weak except on MSVC; CMake omits `n_str.c` on platforms with native `strlcpy`/`strlcat` to avoid fortified macro conflicts.
- Performance shape: O(strlen(src)).

## Direct Call Sites

Exact source search `rg -n "\bstrlcpy\b" -g '!source-research/**' -g '!docs/**'` found:

- Build/config and declarations: `CMakeLists.txt:49`, `CMakeLists.txt:68`-`CMakeLists.txt:72`, `note.h:1152`, `note.h:1154`, `n_str.c:31`/`n_str.c:33`.
- JSON printing: `n_cjson.c:433`, `n_cjson.c:1193`, `n_cjson.c:1201`, `n_cjson.c:1209`.
- MD5 string conversion: `n_md5.c:310`, `n_md5.c:320`.
- Binary/status helpers: `n_helpers.c:408`, `n_helpers.c:1376`, `n_helpers.c:1399`, `n_helpers.c:1435`, `n_helpers.c:1663`, `n_helpers.c:1680`.
- Time/location/cache helpers: `n_helpers.c:822`-`n_helpers.c:824`, `n_helpers.c:909`, `n_helpers.c:919`, `n_helpers.c:921`, `n_helpers.c:922`, `n_helpers.c:1161`, `n_helpers.c:1198`, `n_helpers.c:1200`, `n_helpers.c:1509`.
- Environment/service/contact/route helpers: `n_helpers.c:1308`, `n_helpers.c:1319`, `n_helpers.c:1592`-`n_helpers.c:1595`, `n_helpers.c:1605`, `n_helpers.c:1608`, `n_helpers.c:1611`, `n_helpers.c:1614`, `n_helpers.c:1776`, `n_helpers.c:2151`, `n_helpers.c:2248`, `n_helpers.c:2251`, `n_helpers.c:2254`, `n_helpers.c:2257`.

Test-only occurrences were observed but not researched as standalone targets.

## Role In Callers

Callers use `strlcpy` to bounded-copy generated JSON literals, MD5 hex strings, status strings, cached service/location/contact fields, request names, and user-visible helper outputs. For the active MD5 slice, `NoteMD5HashString` uses it to copy the stack-built hex string into the caller-provided buffer with truncation semantics inherited from `strlcpy`.

## Tests Observed

None for this fallback as a standalone production symbol. Focused tests that use `strlcpy` as support code were intentionally not treated as test coverage for the SDK fallback.

## Graph Links

- Graph node id: `function:n_str.c:31:strlcpy`
- Structured graph: `inventory/dependency-graph.json`
- Verified grouped incoming fanout from `n_cjson.c`, `n_md5.c`, and `n_helpers.c`.

## Evidence Gaps

No unresolved source gaps for this node. Platform-native `strlcpy` behavior is represented as a build/config variant rather than implementation source in this repository.
