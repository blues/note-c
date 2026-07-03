# `JParseWithOpts`

## Status

Finalized. Source-inspected public configurable JSON parser.

## Identity

- Kind: function
- Definition: `n_cjson.c:902`
- Declaration/interface: `n_cjson.h:188`
- Signature/API shape: `J *JParseWithOpts(const char *value, const char **return_parse_end, Jbool require_null_terminated)`
- Visibility: public cJSON/note-c JSON API
- Public/exported names: `JParseWithOpts`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:194:_jNew_Item` finalized
  - `function:n_cjson.c:1129:_parse_value` finalized
  - `function:n_cjson.c:870:_buffer_skip_whitespace` finalized
  - `function:n_cjson.c:888:_skip_utf8_bom` finalized
  - `macro:n_cjson.c:252:buffer_at_offset` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: `strlen`
- Data/type/global dependencies: `parse_buffer`, `global_error`

## Behavior

Resets the global parse error, rejects null input, initializes a parse buffer over the input including the trailing NUL, allocates a root item with `_jNew_Item`, and parses the root value after skipping an optional UTF-8 BOM and leading whitespace. If `require_null_terminated` is true, it skips trailing whitespace and requires the current byte to be NUL. On success it optionally stores `return_parse_end` at the current parse position and returns the root item. On failure it deletes any partial item, sets `return_parse_end` and `global_error` to the best known failure position, and returns NULL.

## Important Boundaries

- Preconditions and assumptions: `value` must be a NUL-terminated string.
- Error/null/empty behavior: null input or parse failure returns NULL and records error state when input is non-null.
- Ownership and lifetime: on success caller owns the returned tree and must delete it with `JDelete`; failure cleans up partial allocation.
- Concurrency/global state: writes `global_error`, so parse error reporting is process-global.
- Performance shape: linear in input size plus allocation for parsed nodes and strings.
- Portability/platform constraints: parser recursion is bounded by `N_CJSON_NESTING_LIMIT`.

## Direct Call Sites

Exact source search found production caller `JParse`; tests also call it through fakes in `NoteRequestResponseJSON_test.cpp`.

## Role In Callers

This is the full parser engine behind `JParse`; `_noteTransactionShouldLock` reaches it through `JParse` when parsing Notecard responses.

## Tests Observed

`NoteRequestResponseJSON_test.cpp` calls `JParseWithOpts` via a fake `JParse`; `JSON_number_handling_test.cpp`, `JPrintUnformatted_test.cpp`, `NoteTransaction_test.cpp`, and `NotePing_test.cpp` exercise it through `JParse`.

## Graph Links

- Graph node id: `function:n_cjson.c:902:JParseWithOpts`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No production direct call with `require_null_terminated=true` was found in source search.
