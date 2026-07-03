# JSON Printing Closure Evidence Report

Scope: source-only sidecar inspection of the JSON printing closure around `n_cjson.c:983` `_print` and `n_cjson.c:1051` `JPrintUnformatted`. Canonical artifacts were not edited. Existing architecture docs, README files, and changelogs were not used as evidence.

## Source Locations

- `n_cjson.c:351` `_ensure`: printbuffer capacity check/growth helper.
- `n_cjson.c:400` `_update_offset`: advances `printbuffer.offset` by `strlen(buffer + offset)`.
- `n_cjson.c:412` `_print_number`: serializes `JNumber`.
- `n_cjson.c:744` `_print_string_ptr`: serializes and escapes C strings.
- `n_cjson.c:856` `_print_string`: item wrapper around `_print_string_ptr`.
- `n_cjson.c:981` `cjson_min`: local min macro used during final copy.
- `n_cjson.c:983` `_print`: allocator-owned print entry used by public wrappers.
- `n_cjson.c:1051` `JPrintUnformatted`: public unformatted wrapper.
- `n_cjson.c:1179` `_print_value`: type dispatcher for printable `J` values.
- `n_cjson.c:1341` `_print_array`: recursive array printer.
- `n_cjson.c:1507` `_last_non_omitted_object`: lookahead for omit-empty comma placement.
- `n_cjson.c:1523` `_print_object`: recursive object printer.
- `n_cjson_helpers.c:387` `JGetItemType`: detailed type classifier used by omit-empty object printing.
- `n_atof.c:71` `JAtoN`: string-to-number conversion used by `JGetItemType` string classification.
- `n_cjson.h:185-203`: public memory/printing ownership contract and declarations.
- `n_cjson.h:310`: `JConvertToJSONString` macro alias to `JPrintUnformatted`.
- `note.h:1400-1413`, `note.h:1431`, `note.h:1464`: detailed `JTYPE_*` constants and helper declarations.

## Dependency Shape

- `JPrintUnformatted(item)` rejects `NULL`, then calls `_print(item, false, false)`.
- `_print` allocates a 128-byte printbuffer with `_Malloc`, calls `_print_value`, calls `_update_offset`, allocates an exact result buffer, copies `min(buffer.length, offset + 1)`, forces a trailing NUL, frees the working buffer, and returns caller-owned text.
- `_print_value` dispatches by `item->type & 0xFF`:
  - null/false/true/raw write directly through `_ensure`.
  - number calls `_print_number`.
  - string calls `_print_string` -> `_print_string_ptr`.
  - array calls `_print_array`.
  - object calls `_print_object`.
- `_print_array` writes delimiters and separators, calls `_print_value` for each child, and then `_update_offset`.
- `_print_object` writes delimiters, optional formatting indentation, key strings via `_print_string_ptr`, values via `_print_value`, comma placement via `_last_non_omitted_object`, and `_update_offset` after key/value writes.
- Omit-empty object printing is conditional on `printbuffer.omitempty`; `JPrintUnformatted` sets this false, while `JPrintUnformattedOmitEmpty` and `JPrintPreallocatedOmitEmpty` set it true.
- `_last_non_omitted_object` and `_print_object` omit only fields classified as `JTYPE_BOOL_FALSE`, `JTYPE_NUMBER_ZERO`, or `JTYPE_STRING_BLANK`. String values like `"0"` and `"false"` are classified by `JGetItemType` but are not omitted.

## Cycles

- Recursive data cycle: `_print_value -> _print_array -> _print_value` for nested arrays.
- Recursive data cycle: `_print_value -> _print_object -> _print_value` for nested objects.
- Omit-empty side path: `_print_object -> _last_non_omitted_object -> JGetItemType -> JAtoN` only when omit-empty logic inspects string siblings.
- No source-level call cycle was observed through `JGetItemType` or `JAtoN`.

## Direct Callers

- `_print`: `JPrint` (`n_cjson.c:1032`), `JPrintUnformatted` (`n_cjson.c:1051`), `JPrintUnformattedOmitEmpty` (`n_cjson.c:1059`), plus test-only access through `test/include/test_static.h:27` and `test/src/NoteRequestResponseJSON_test.cpp:55`.
- `JPrintUnformatted`: production callers at `n_request.c:365`, `n_request.c:437`, and `n_request.c:869`; macro alias declaration at `n_cjson.h:310`; focused tests in `test/src/JPrintUnformatted_test.cpp`, `test/src/JSON_number_handling_test.cpp`, `test/src/NotePing_test.cpp`, and faked request tests.
- `_print_value`: `_print`, `JPrintBuffered`, `_printPreallocated`, `_print_array`, and `_print_object`.
- `_ensure`: `_print_number`, `_print_string_ptr`, `_print_value`, `_print_array`, and `_print_object`.
- `_update_offset`: `_print`, `_print_array`, and `_print_object`.
- `_print_number`, `_print_string`, `_print_array`, `_print_object`: direct caller is `_print_value`.
- `_print_string_ptr`: `_print_string` and `_print_object`.
- `_last_non_omitted_object`: `_print_object`.
- `JGetItemType`: `JGetType`, `_last_non_omitted_object`, and `_print_object`.
- `JAtoN`: `_parse_number`, `JGetItemType`, `NoteGetEnvNumber`, and focused `JAtoN` tests.

## Ownership, Errors, and Bounds

- Returned `JPrintUnformatted` text is allocator-owned. Header comments state callers free printed results with stdlib free, hook free, or `JFree`; production callers observed use `_Free` after use.
- `_print` owns the temporary buffer and the exact result buffer until success. On any failure it frees both if allocated and returns `NULL`.
- `_ensure` rejects null buffers, invalid offsets, sizes above `INT_MAX`, and growth requests when `noalloc` is true. On growth allocation failure it frees the old buffer, zeros length, nulls the buffer pointer, and returns `NULL`.
- `_update_offset` assumes the fragment at the current offset is NUL-terminated. The inspected print helpers write a terminating NUL after each fragment that requires later offset discovery.
- `_print_number` prints non-finite values as `null`, uses `JNtoA` for non-integral numbers and `JItoA` for integral values, normalizes the locale decimal point to `.`, and checks the temporary number buffer length before copying.
- `_print_string_ptr` handles `NULL` input as `""`, escapes quotes, backslashes, JSON control characters, and other bytes below 32 as `\u00XX`. It requires NUL-terminated input and does not independently cap the first-pass scan length.
- `_print_array` and `_print_object` are recursive and have no print-time nesting limit in this closure; deeply nested in-memory trees can consume stack.
- `_print_object` default formatted indentation uses `PRINT_TAB_CHARS 4`. The `PRINT_TAB_CHARS == 0` branch references `output_buffer_offset`, which is not declared, but that branch is dead under the current source definition.
- Formatted object indentation computes `int needed` from `output_buffer->depth`; this is irrelevant for `JPrintUnformatted` but is a bounds/variant point for formatted printing on very deep objects.

## Focused Tests Observed

- `test/src/JPrintUnformatted_test.cpp:24-58`: parses a mixed object and verifies `JPrintUnformatted` returns non-null, non-empty output; it does not assert exact JSON text.
- `test/src/JSON_number_handling_test.cpp:220-412`: exact or approximate assertions for unformatted numeric marshalling, including zero, integer boundaries, rounded large values, small floating point, and a Unix timestamp.
- `test/src/JGetType_test.cpp:20-148`: validates detailed `JTYPE_*` classifications used by omit-empty logic, including false, numeric zero, blank string, numeric strings, boolean strings, object, array, and invalid item.
- `test/src/NoteRequestResponseJSON_test.cpp:27-55` fakes `JPrintUnformatted` and maps the fake to `_print(json, false, false)` for request-flow tests; later tests cover `NULL` print output and allocation failure around error-document serialization.
- `test/src/NotePing_test.cpp:116-123` uses `JPrintUnformatted` in a response helper; production `NotePing` uses it for request serialization and overwrites the returned NUL with newline before transport.
- No focused test was found for `JPrintUnformattedOmitEmpty`, `JPrintPreallocatedOmitEmpty`, exact object comma placement under omitted fields, or string escaping exact output in this closure.

## Graph Gaps / Artifact Notes

- Candidate graph facts to ensure in canonical artifacts: `JConvertToJSONString -> JPrintUnformatted` macro alias; test-only `_print` exposure through `NOTE_C_STATIC`/`test_static.h`; recursive mutual edges through `_print_value`/`_print_array` and `_print_value`/`_print_object`; conditional omit-empty edges from `_print_object` to `JGetItemType` and from `JGetItemType` to `JAtoN`.
- Exact source search found no production use of `JConvertToJSONString`; it should be modeled as a public alias even when uncalled.
- Existing canonical graph files were not reconciled in this sidecar pass because the task constrained the evidence policy to source files and focused tests and forbade canonical artifact edits.

## Uncertainty / Gaps

- No compiler or AST substrate was run for this sidecar report; caller/callee coverage is exact-source-search based.
- Allocation-failure paths in `_ensure` and `_print` were inspected from source but not executed here.
- Omit-empty semantics were inspected from source and adjacent `JGetType` tests, but no direct omit-empty print test was found.
- Very large string length overflow behavior in `_print_string_ptr` was not exhaustively bounded beyond the later `_ensure` `INT_MAX` cap.
- Formatted-printing behavior was included only where it shares closure code; `JPrintUnformatted` itself uses `format=false`.

## Recommended Finalization Order

1. Finalize leaf/bounds helpers: `cjson_min`, `_ensure`, `_update_offset`, `_get_decimal_point`, `JNtoA`, `JItoA`, and the limited `JAtoN` facts needed by classification.
2. Finalize classification side path: `JGetItemType` with `JGetType` focused tests, then `_last_non_omitted_object`.
3. Finalize scalar printers: `_print_number`, `_print_string_ptr`, and `_print_string`.
4. Finalize `_print_value` primitive dispatch and raw-value handling.
5. Finalize recursive containers as a paired slice: `_print_array`, `_print_object`, and their `_print_value` back edges.
6. Finalize allocator-owned entry points: `_print`, `JPrintUnformatted`, `JPrint`, `JPrintUnformattedOmitEmpty`, `JPrintBuffered`, and preallocated variants.
7. Finalize public fanout: `n_request.c` serialization/error-document/ping callers, `JConvertToJSONString` alias exposure, and focused test evidence batches.
