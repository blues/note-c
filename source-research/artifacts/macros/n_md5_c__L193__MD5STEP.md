# `MD5STEP`

## Status

Finalized. Source-inspected MD5 round-step macro.

## Identity

- Kind: macro
- Definition: `n_md5.c:193`
- Signature/API shape: `#define MD5STEP(f, w, x, y, z, data, s) ( w += f(x, y, z) + data, w &= 0xffffffff, w = w<<s | w>>(32-s), w += x )`
- Visibility: local to `n_md5.c`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: function-like macro argument `f`, passed as `F1`, `F2`, `F3`, or `F4` from `NoteMD5Transform`
- External/system dependencies: primitive integer addition, masking, shifts, and bitwise OR
- Data/type/global dependencies: none
- Graph repair: added verified source-inspected macro-use edges from `NoteMD5Transform` to `F1`-`F4` and parameterized possible macro edges from `MD5STEP` to `F1`-`F4`.

## Behavior

Performs one MD5 round update on working word `w`: it adds the selected round function result and message/constant data, masks the accumulator to 32 bits, rotates left by `s`, then adds `x`.

## Important Boundaries

- Preconditions and assumptions: macro mutates `w`; callers must pass lvalue working words and integer expressions valid for the MD5 round.
- Error/null/empty behavior: not applicable.
- Ownership and lifetime: no memory ownership.
- Concurrency/global state: no state.
- Portability/platform constraints: explicitly masks `w` to 32 bits before rotation, matching the file's portability strategy for `unsigned long`.
- Performance shape: inline macro expression; no loops or allocation.

## Direct Call Sites

Exact source search `rg -n "\bMD5STEP\b" n_md5.c` found the macro definition at `n_md5.c:193` and 64 invocations in `NoteMD5Transform`:

- `n_md5.c:216`-`n_md5.c:231` use `F1`
- `n_md5.c:233`-`n_md5.c:248` use `F2`
- `n_md5.c:250`-`n_md5.c:265` use `F3`
- `n_md5.c:267`-`n_md5.c:282` use `F4`

## Role In Callers

`NoteMD5Transform` is the only production source caller. The macro encodes each of the 64 MD5 round operations in that transform.

## Tests Observed

None. Macro behavior is covered only through higher-level MD5 callers in this research slice.

## Graph Links

- Graph node id: `macro:n_md5.c:193:MD5STEP`
- Structured graph: `inventory/dependency-graph.json`
- Verified incoming edge: `function:n_md5.c:201:NoteMD5Transform` -> `macro:n_md5.c:193:MD5STEP`

## Evidence Gaps

No unresolved source gaps for this node.
