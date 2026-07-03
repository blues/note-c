# `JPrintUnformatted`

## Status

Finalized. Source-inspected public unformatted JSON serializer.

## Identity

- Kind: function
- Definition: `n_cjson.c:1051`
- Declaration/interface: `n_cjson.h:195`
- Signature/API shape: `char *JPrintUnformatted(const J *item)`
- Visibility: public cJSON/note-c JSON API
- Public/exported names: `JPrintUnformatted`, macro alias `JConvertToJSONString` in `n_cjson.h:310`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:983:_print` finalized
- External/system dependencies: none
- Data/type/global dependencies: caller-provided `J` tree

## Behavior

Returns NULL for a null item. Otherwise delegates to `_print(item, false, false)`, producing heap-allocated compact JSON with no formatting whitespace and no omit-empty filtering. The returned string is owned by the caller and must be freed through the configured JSON free path.

## Important Boundaries

- Preconditions and assumptions: input `J` tree must be valid for the print engine.
- Error/null/empty behavior: null input or `_print` failure returns NULL.
- Ownership and lifetime: transfers ownership of the allocated result to the caller.
- Concurrency/global state: allocator/free behavior follows configured note-c hooks through `_print`.
- Performance shape: follows `_print` and `_ensure` growth behavior.
- Portability/platform constraints: this is the serialization mode used by request/transaction paths before adding the newline expected by transport hooks.

## Direct Call Sites

Exact source search found production callers in `NoteRequestResponseJSON`, `_noteTransactionShouldLock`, and `NotePing`. Tests call or fake it in `JPrintUnformatted_test.cpp`, `JSON_number_handling_test.cpp`, `NotePing_test.cpp`, and `NoteRequestResponseJSON_test.cpp`.

## Role In Callers

`_noteTransactionShouldLock` serializes the request object through this function before timeout, CRC, transport, and retry handling. Raw JSON helper paths also use it to turn generated error documents back into response JSON.

## Tests Observed

`JPrintUnformatted_test.cpp` exercises mixed scalar/container output. `JSON_number_handling_test.cpp` gives focused number serialization coverage. `NoteRequestResponseJSON_test.cpp` fakes this function for raw JSON request/response paths.

## Graph Links

- Graph node id: `function:n_cjson.c:1051:JPrintUnformatted`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused tests were inspected for allocation failure through this public wrapper or for the `JConvertToJSONString` alias.
