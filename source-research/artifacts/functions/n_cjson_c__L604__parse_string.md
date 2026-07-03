# `_parse_string`

## Status

Finalized. Source-inspected JSON string parser and unescaper.

## Identity

- Kind: function
- Definition: `n_cjson.c:604`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `Jbool _parse_string(J * const item, parse_buffer * const input_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_cjson.c:252:buffer_at_offset` finalized
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_cjson.c:504:_utf16_literal_to_utf8` finalized
  - `macro:n_lib.h:242:_Free` finalized
- External/system dependencies: none
- Data/type/global dependencies: `J.type`, `J.valuestring`, `JString`

## Behavior

Requires the current byte to be a double quote, scans to the closing quote while accounting for escape sequences, allocates an overestimated output buffer, and then copies/unescapes the string. It handles `\b`, `\f`, `\n`, `\r`, `\t`, escaped quote, backslash, slash, and `\uXXXX` sequences through `_utf16_literal_to_utf8`. On success it NUL-terminates the output, stores it as `item->valuestring`, sets type to `JString`, advances the parse offset past the closing quote, and returns true. Failures free the allocated output and set the parse offset to the failing input pointer when available.

## Important Boundaries

- Preconditions and assumptions: internal callers pass valid item and input buffer.
- Error/null/empty behavior: malformed strings, bad escapes, truncated input, or allocation failure return false.
- Ownership and lifetime: on success the parsed `J` owns the allocated string; on failure this helper frees its temporary output.
- Concurrency/global state: allocator/free behavior follows note-c hooks.
- Performance shape: two passes over the string literal plus bounded escape conversion.
- Portability/platform constraints: unicode escapes are converted to UTF-8 manually.

## Direct Call Sites

Exact source search found direct callers in `_parse_value` and `_parse_object` for object member names.

## Role In Callers

It parses both string values and object keys; `_parse_object` moves a parsed key from `valuestring` to `string`.

## Tests Observed

`JPrintUnformatted_test.cpp`, `NoteResponseError_test.cpp`, and several request tests exercise string parsing indirectly through `JParse`.

## Graph Links

- Graph node id: `function:n_cjson.c:604:_parse_string`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused direct tests were identified for invalid escape offsets or surrogate-pair handling.
