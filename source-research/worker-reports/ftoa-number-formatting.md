# ftoa number formatting worker report

Scope: `n_ftoa.c:56` `JNtoA`, its file-local helpers/macros used by that path, declarations in public/internal headers, and direct production callers/focused tests. Documentation/changelog/architecture files were not read. Canonical source-research artifacts were not edited.

## Source locations

- Public number type/config: `note.h:48-58` selects `NOTE_C_SINGLE_PRECISION` when float/double exponent ranges match; `note.h:84-88` typedefs `JNUMBER` as `float` in that mode and `double` otherwise.
- Public formatter contract: `note.h:1443-1455` defines `JNTOA_PRECISION` as `16`, `JNTOA_MAX` as `44`, and declares `char * JNtoA(JNUMBER f, char * buf, int precision)`.
- Internal include path: `n_lib.h:19` includes `note.h`; `n_ftoa.c:22-24` includes `n_lib.h`, `<stdint.h>`, and `<math.h>`.
- Formatter implementation: `n_ftoa.c:26-34` flag macros; `n_ftoa.c:36-44` static helper prototypes; `n_ftoa.c:45-50` `OUTCHAR`; `n_ftoa.c:56-71` `JNtoA`; `n_ftoa.c:73-376` `fmtflt`; `n_ftoa.c:378-413` `fmtstr`; `n_ftoa.c:415-423` `printsep`/`getnumsep`; `n_ftoa.c:426-445` `getexponent`; `n_ftoa.c:448-459` `convert`; `n_ftoa.c:462-483` `cast`; `n_ftoa.c:486-494` `myround`; `n_ftoa.c:497-510` `mypow10`.
- JSON print caller: `n_cjson.c:412-471` `_print_number`; public print entries are `JPrint`/`JPrintUnformatted`/`JPrintUnformattedOmitEmpty` at `n_cjson.c:1032-1064`, `JPrintBuffered` at `n_cjson.c:1067-1095`, and `JPrintPreallocated*` at `n_cjson.c:1097-1125`.
- JSON declarations: `n_cjson.h:192-203` declares the public print entries; `n_cjson.h:78-96` defines `struct J` with `valueint` and `valuenumber`.

## Behavior summary

`JNtoA` is a no-allocation public wrapper. It defaults negative precision to `JNTOA_PRECISION` (`16`), uses only `PRINT_F_TYPE_G`, calls `fmtflt` with a fixed `JNTOA_MAX` output size, converts `fmtflt`'s integer-overflow sentinel to the string `"0"`, NUL-terminates, and returns the caller's `buf`.

`fmtflt` implements a constrained `%g`-style conversion. It handles sign and direct NaN/Infinity text, decrements `%g` precision to fractional precision, chooses fixed or exponent style based on the C99 `%g` exponent rule, clamps precision by `sizeof(uintmax_t)` to 9/19/38 digits, splits absolute value into integer/fractional parts, rounds the scaled fraction, carries into the integer/exponent when needed, converts integer/fraction/exponent digits in reverse, omits trailing fractional zeros for `%g`, and emits characters through `OUTCHAR`.

Helper roles:

- `OUTCHAR` writes only when `len + 1 < size`, then always increments `len`.
- `fmtstr` is used for direct `JNtoA` NaN/Infinity text and padding, but JSON printing bypasses non-finite numbers before `JNtoA`.
- `printsep`/`getnumsep` implement comma grouping but are inactive for `JNtoA` because `PRINT_F_QUOTE` is never set by `JNtoA`.
- `getexponent` computes a decimal exponent with loop bounds `[-1022, 1023]`.
- `convert` writes unterminated reverse digits for unsigned integer values.
- `cast` returns `UINTMAX_MAX` as an overflow sentinel and compensates for platforms that incorrectly round long-double-to-integer casts upward.
- `myround` is half-up rounding over `cast`.
- `mypow10` multiplies/divides by 10 iteratively for positive/negative exponents.

JSON printing uses this formatter only for non-integral numeric values. `_print_number` rejects null inputs, allocates a local `number_buffer[JNTOA_MAX]`, maps NaN/Infinity to `"null"` with `strlcpy`, calls `JNtoA(vnum, nbuf, -1)` when `vnum != (JNUMBER)vint`, otherwise calls `JItoA(vint, nbuf)`, validates `strlen(nbuf) <= JNTOA_MAX - 1`, reserves output via `_ensure`, copies bytes, normalizes the locale decimal separator back to `.`, and advances the print offset. Number object construction influences that branch: parse/set/create-number paths saturate `valueint` for out-of-range `JNUMBER` (`n_cjson.c:300-337`, `n_cjson.c:2330-2347`), while `JCreateInteger` stores exact integer and numeric forms (`n_cjson.c:2349-2357`).

## Project-local callees

- `JNtoA` -> `fmtflt`.
- `fmtflt` -> `fmtstr`, `getexponent`, `mypow10`, `cast`, `myround`, `convert`, `getnumsep`, `printsep`, and macro `OUTCHAR`; external/library calls are `isnan` and `isinf`.
- `myround` -> `cast`; `printsep` and `fmtstr` -> `OUTCHAR`.
- JSON caller `_print_number` -> `_get_decimal_point`, `strlcpy`, `JNtoA`, `JItoA`, `strlen`, `_ensure`; `_ensure` reallocates through `_Malloc`/`_Free`, which map to `NoteMalloc`/`NoteFree` via `n_lib.h:241-242`.

## Direct callers

- Direct `JNtoA` production callers found by exact symbol search:
  - `n_cjson.c:438-440` in `_print_number`, for non-integral JSON numbers.
  - `n_helpers.c:1250-1254` in `NoteSetEnvDefaultNumber`, formatting a default service environment value before `NoteSetEnvDefault`.
  - `n_helpers.c:1265-1270` in `NoteGetEnvNumber`, formatting the default value before `NoteGetEnv`, then parsing the result with `JAtoN`.
- Direct `_print_number` production caller: `_print_value` dispatches `JNumber` to `_print_number` at `n_cjson.c:1179-1213`.
- Production entry paths into `_print_value`: `_print` at `n_cjson.c:983-1004`, `JPrintBuffered` at `n_cjson.c:1067-1089`, `_printPreallocated` at `n_cjson.c:1097-1115`, recursive array printing at `n_cjson.c:1341-1366`, and recursive object printing at `n_cjson.c:1523-1607`.

## Ownership, error, and bounds behavior

- `JNtoA` does not allocate and does not check `buf` for null; callers must provide a writable `JNTOA_MAX` buffer.
- `JNtoA` reports only `cast`/integer overflow from `fmtflt`, and converts that to `"0"`. It has no separate truncation/error return.
- `OUTCHAR` avoids writing a character unless space remains for a terminator, but it still increments `len`. If a formatted result ever reaches/exceeds `size`, `JNtoA` will write `buf[len] = '\0'` using the counted length, not a clamped index. Observed default `float`/`double` `%g` paths appear intended to fit `JNTOA_MAX`, but this is a boundary to verify for unusual precision/platform combinations, especially `sizeof(uintmax_t) == 16`.
- `_print_number` has normal boolean failure boundaries for null item/output buffer, formatted length greater than `JNTOA_MAX - 1`, and `_ensure` failure.
- `_ensure` returns `NULL` for invalid buffers, offset beyond length, `needed > INT_MAX`, noalloc/preallocated overflow, or allocation failure; on allocation failure it frees the old print buffer and clears it (`n_cjson.c:351-397`).
- JSON print result ownership is outside `JNtoA`: public `JPrint*` allocation is through the note-c allocator hooks, and callers free returned strings with `JFree`/configured free as declared near `n_cjson.h:185-203`.

## Focused tests observed

- `test/src/JNtoA_test.cpp:22-67` calls `JNtoA` for approximate max/min float or double values depending on `NOTE_C_SINGLE_PRECISION`, but the assertion parses `expectedNumStrs[i]`, not the generated `numStr`. This is weak evidence because the formatter output is not compared or parsed.
- `test/src/JSON_number_handling_test.cpp:220-410` is the closest JSON printing evidence. It exercises `JPrintUnformatted` for zero, exact `JINTEGER_MAX`, out-of-range positive/negative `JNUMBER` values with float/double conditional expectations, `JINTEGER_MIN`, a small fractional value, and a Unix timestamp integer. Exact string assertions cover most cases; the small-float case parses the emitted number but checks `(numValue - SMALL_FLOAT) < 1e-11` instead of an absolute difference.
- `test/src/NoteSetEnvDefaultNumber_test.cpp:34-49` covers the environment setter caller with `-123456`.
- `test/src/NoteGetEnvNumber_test.cpp:44-63` covers the environment getter caller with existing and default values.
- `test/src/JPrintUnformatted_test.cpp:24-60` covers general `JPrintUnformatted` non-null output for a mixed JSON object but does not assert number formatting text.
- Tests were observed by source inspection only; this sidecar did not execute them.

## Uncertainty and gaps

- Direct `JNtoA` test coverage is currently ineffective for output correctness because it ignores `numStr`.
- No focused test observed for direct `JNtoA` NaN/Infinity output, JSON NaN/Infinity-to-`null`, explicit precision values, rounding carry boundaries such as `9.999...`, locale decimal separator replacement under `ENABLE_LOCALES`, caller-provided undersized buffers, or the `sizeof(uintmax_t) == 16` precision/buffer boundary.
- `fmtstr`, `printsep`, and `getnumsep` are present in the local formatter but are not exercised by JSON printing with current `JNtoA` flags; `fmtstr` remains reachable only through direct non-finite `JNtoA`.
- No compiler/AST substrate was run for this sidecar; caller/callee coverage is from exact source search and narrow source inspection.

## Proposed artifact statuses

- `n_ftoa.c::JNtoA`: identity verified; behavior inspected; local callees verified; direct production callers verified; focused tests observed but weak for direct formatter output; proposed `finalized` candidate with confidence high after conductor merge.
- `n_ftoa.c::fmtflt`: identity verified; behavior inspected for the `JNtoA` `%g` path; inactive generic formatting flags noted; proposed `finalized` candidate for JSON/default `JNtoA` behavior, `inspected` for unused flag variants.
- `n_ftoa.c::fmtstr`: identity verified; behavior inspected; proposed `inspected` because JSON printing bypasses its non-finite path.
- `n_ftoa.c::printsep` and `n_ftoa.c::getnumsep`: identity verified; behavior inspected; proposed `inspected/inactive-for-JNtoA` because `PRINT_F_QUOTE` is not set.
- `n_ftoa.c::getexponent`, `convert`, `cast`, `myround`, `mypow10`, and `OUTCHAR`: identity verified; behavior and call relationships inspected; proposed `finalized` candidates for this slice.
- `n_cjson.c::_print_number`: identity verified; behavior, callees, callers, ownership/error/bounds inspected; focused JSON marshalling tests observed; proposed `finalized` candidate with confidence high.
- `n_helpers.c::NoteSetEnvDefaultNumber` and `n_helpers.c::NoteGetEnvNumber`: direct caller identities and basic behavior inspected; focused tests observed; proposed `inspected`.

Suggested graph/status edges for conductor merge only: `JNtoA calls fmtflt`; `fmtflt calls {fmtstr,getexponent,mypow10,cast,myround,convert,getnumsep,printsep}`; `myround calls cast`; `_print_number calls JNtoA` and `JItoA`; `_print_value calls _print_number`; public `JPrint*` entries and recursive array/object printers reach `_print_value`; environment helpers call `JNtoA`.
